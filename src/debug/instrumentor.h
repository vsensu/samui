#ifndef SAMUI_INSTRUMENTOR_H_
#define SAMUI_INSTRUMENTOR_H_

#include <algorithm>
#include <chrono>
#include <fstream>
#include <iomanip>
#include <mutex>
#include <sstream>
#include <string>
#include <thread>

#include "../core/core.h"
#include "../log/log.h"

namespace samui {
using FloatingPointMicroseconds = std::chrono::duration<double, std::micro>;

struct SAMUI_API ProfileResult {
  std::string Name;

  FloatingPointMicroseconds Start;
  std::chrono::microseconds ElapsedTime;
  std::thread::id           ThreadID;
};

struct InstrumentationSession {
  std::string Name;
};

class SAMUI_API Instrumentor {
 public:
  Instrumentor(const Instrumentor&) = delete;
  Instrumentor(Instrumentor&&) = delete;

  void BeginSession(const std::string& name,
                    const std::string& filepath = "results.json") {
    std::lock_guard lock(m_Mutex);
    if (m_CurrentSession) {
      // If there is already a current session, then close it before beginning
      // new one. Subsequent profiling output meant for the original session
      // will end up in the newly opened session instead.  That's better than
      // having badly formatted profiling output.
      if (Log::GetEngineLogger())  // Edge case: BeginSession() might be before
                                   // Log::Init()
      {
        SAMUI_ENGINE_ERROR(
            "Instrumentor::BeginSession('{0}') when session '{1}' already "
            "open.",
            name, m_CurrentSession->Name);
      }
      InternalEndSession();
    }
    m_OutputStream.open(filepath);

    if (m_OutputStream.is_open()) {
      m_CurrentSession = new InstrumentationSession({name});
      WriteHeader();
    } else {
      if (Log::GetEngineLogger())  // Edge case: BeginSession() might be before
                                   // Log::Init()
      {
        SAMUI_ENGINE_ERROR("Instrumentor could not open results file '{0}'.",
                           filepath);
      }
    }
  }

  void EndSession() {
    std::lock_guard lock(m_Mutex);
    InternalEndSession();
  }

  void WriteProfile(const ProfileResult& result) {
    std::stringstream json;

    json << std::setprecision(3) << std::fixed;
    json << ",{";
    json << "\"cat\":\"function\",";
    json << "\"dur\":" << (result.ElapsedTime.count()) << ',';
    json << "\"name\":\"" << result.Name << "\",";
    json << "\"ph\":\"X\",";
    json << "\"pid\":0,";
    json << "\"tid\":" << result.ThreadID << ",";
    json << "\"ts\":" << result.Start.count();
    json << "}";

    std::lock_guard lock(m_Mutex);
    if (m_CurrentSession) {
      m_OutputStream << json.str();
      m_OutputStream.flush();
    }
  }

  static Instrumentor& Get() {
    static Instrumentor instance;
    return instance;
  }

 private:
  Instrumentor() : m_CurrentSession(nullptr) {}

  ~Instrumentor() { EndSession(); }

  void WriteHeader() {
    m_OutputStream << "{\"otherData\": {},\"traceEvents\":[{}";
    m_OutputStream.flush();
  }

  void WriteFooter() {
    m_OutputStream << "]}";
    m_OutputStream.flush();
  }

  // Note: you must already own lock on m_Mutex before
  // calling InternalEndSession()
  void InternalEndSession() {
    if (m_CurrentSession) {
      WriteFooter();
      m_OutputStream.close();
      delete m_CurrentSession;
      m_CurrentSession = nullptr;
    }
  }

 private:
  std::mutex              m_Mutex;
  InstrumentationSession* m_CurrentSession;
  std::ofstream           m_OutputStream;
};

class SAMUI_API InstrumentationTimer {
 public:
  InstrumentationTimer(const char* name) : m_Name(name), m_Stopped(false) {
    m_StartTimepoint = std::chrono::steady_clock::now();
  }

  ~InstrumentationTimer() {
    if (!m_Stopped) Stop();
  }

  void Stop() {
    auto endTimepoint = std::chrono::steady_clock::now();
    auto highResStart =
        FloatingPointMicroseconds{m_StartTimepoint.time_since_epoch()};
    auto elapsedTime =
        std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint)
            .time_since_epoch() -
        std::chrono::time_point_cast<std::chrono::microseconds>(
            m_StartTimepoint)
            .time_since_epoch();

    Instrumentor::Get().WriteProfile(
        {m_Name, highResStart, elapsedTime, std::this_thread::get_id()});

    m_Stopped = true;
  }

 private:
  const char*                                        m_Name;
  std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;
  bool                                               m_Stopped;
};

namespace InstrumentorUtils {

template <size_t N>
struct SAMUI_API ChangeResult {
  char Data[N];
};

template <size_t N, size_t K>
constexpr auto SAMUI_API CleanupOutputString(const char (&expr)[N],
                                   const char (&remove)[K]) {
  ChangeResult<N> result = {};

  size_t srcIndex = 0;
  size_t dstIndex = 0;
  while (srcIndex < N) {
    size_t matchIndex = 0;
    while (matchIndex < K - 1 && srcIndex + matchIndex < N - 1 &&
           expr[srcIndex + matchIndex] == remove[matchIndex])
      matchIndex++;
    if (matchIndex == K - 1) srcIndex += matchIndex;
    result.Data[dstIndex++] = expr[srcIndex] == '"' ? '\'' : expr[srcIndex];
    srcIndex++;
  }
  return result;
}
}  // namespace InstrumentorUtils

}  // namespace samui

#define SAMUI_PROFILE 1
#if SAMUI_PROFILE
   // Resolve which function signature macro will be used. Note that this only
// is resolved when the (pre)compiler starts, so the syntax highlighting
// could mark the wrong one in your editor!
#if defined(__GNUC__) || (defined(__MWERKS__) && (__MWERKS__ >= 0x3000)) || \
    (defined(__ICC) && (__ICC >= 600)) || defined(__ghs__)
#define SAMUI_FUNC_SIG __PRETTY_FUNCTION__
#elif defined(__DMC__) && (__DMC__ >= 0x810)
#define SAMUI_FUNC_SIG __PRETTY_FUNCTION__
#elif (defined(__FUNCSIG__) || (_MSC_VER))
#define SAMUI_FUNC_SIG __FUNCSIG__
#elif (defined(__INTEL_COMPILER) && (__INTEL_COMPILER >= 600)) || \
    (defined(__IBMCPP__) && (__IBMCPP__ >= 500))
#define SAMUI_FUNC_SIG __FUNCTION__
#elif defined(__BORLANDC__) && (__BORLANDC__ >= 0x550)
#define SAMUI_FUNC_SIG __FUNC__
#elif defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901)
#define SAMUI_FUNC_SIG __func__
#elif defined(__cplusplus) && (__cplusplus >= 201103)
#define SAMUI_FUNC_SIG __func__
#else
#define SAMUI_FUNC_SIG "SAMUI_FUNC_SIG unknown!"
#endif

#define SAMUI_PROFILE_BEGIN_SESSION(name, filepath) \
  ::samui::Instrumentor::Get().BeginSession(name, filepath)
#define SAMUI_PROFILE_END_SESSION() ::samui::Instrumentor::Get().EndSession()
#define SAMUI_PROFILE_SCOPE_LINE2(name, line)                            \
  constexpr auto fixedName##line =                                       \
      ::samui::InstrumentorUtils::CleanupOutputString(name, "__cdecl "); \
  ::samui::InstrumentationTimer timer##line(fixedName##line.Data)
#define SAMUI_PROFILE_SCOPE_LINE(name, line) \
  SAMUI_PROFILE_SCOPE_LINE2(name, line)
#define SAMUI_PROFILE_SCOPE(name) SAMUI_PROFILE_SCOPE_LINE(name, __LINE__)
#define SAMUI_PROFILE_FUNCTION() SAMUI_PROFILE_SCOPE(SAMUI_FUNC_SIG)
#else
#define SAMUI_PROFILE_BEGIN_SESSION(name, filepath)
#define SAMUI_PROFILE_END_SESSION()
#define SAMUI_PROFILE_SCOPE(name)
#define SAMUI_PROFILE_FUNCTION()
#endif

#endif  // SAMUI_INSTRUMENTOR_H_