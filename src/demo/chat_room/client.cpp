#include <engine/common/Application.h>
#include <engine/network/net.h>
#include <imgui.h>

#include "msg_type.h"

using ParentClientType = samui::net::client_interface<Message>;
class ChatRoomClient : public Application, public ParentClientType
{
public:
private:
    void RenderUI () override
    {
        ImGui::Begin("Chat Root");
        // ImGui::ShowDemoWindow();

        if (is_connected())
        {
            if (!incoming().empty())
            {
                auto msg = incoming().pop_front().msg;
                switch (msg.header.id)
                {
                case Message::BroadCastUserMessage:
                {
                    int bytes;
                    msg >> bytes;
                    content_recv += std::to_string(bytes);
                }
                break;
                }
            }
        }
        ImGui::Text(content_recv.c_str());

        static char text[1024 * 16] = "";
        static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
        ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
        // ImGui::InputTextMultiline("##source", content_to_send.c_str(), content_to_send.size(), ImVec2(-FLT_MIN, ImGui::GetTextLineHeight() * 16), flags);
        ImGui::Text(text);
        if(ImGui::Button("Send"))
        {
            // std::vector<char> cstr(str.c_str(), str.c_str() + str.size() + 1);
            // content_to_send = text;
            content_to_send = std::vector<char>(std::begin(text), std::end(text));
            content_to_send.push_back('\0');
            
            send_content();
        }
        ImGui::End();
    }

    void send_content()
    {
        samui::net::message<Message> msg;
        msg.header.id = Message::BroadCastUserMessage;

        // std::vector<char> cstr(str.c_str(), str.c_str() + str.size() + 1);
        msg << 123;
        send(msg);
    }

private:
    std::vector<char> content_to_send;
    std::string content_recv;
};

int main(int argc, char **argv)
{
    ChatRoomClient app;
    app.connect("127.0.0.1", 60000);
    return app.Run();
}
