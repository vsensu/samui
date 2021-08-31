-- 导入配置模块
import("core.project.config")

-- 导入工程模块
import("core.project.project")

function main(...)
    -- https://xmake.io/#/zh-cn/manual/plugin_task

    -- 先加载工程配置
    config.load()

    local project_dir = "$(projectdir)"
    local build_dir = ""

    local target = project.target("engine")
    if target then
        -- 获取目标名
        print(target:name())

        -- 获取目标目录, 2.1.9版本之后才有
        print(target:targetdir())

        -- 获取目标文件名
        print(target:targetfile())

        -- 获取目标类型，也就是：binary, static, shared
        print(target:targetkind())

        -- 获取目标名
        print(target:name())

        -- 获取目标依赖
        print(target:get("deps"))

        build_dir = path.join(target:targetdir())
    end

    local config_path = path.join("$(projectdir)", "build" , "config.txt")
    -- 打开文件：w 为写模式, a 为追加写模式
    local file = io.open(config_path, "w")
    if file then
        -- 使用xmake扩展的格式化传参写入一行，无换行符，并且支持内置变量
        file:printf(project_dir .. "\n", "xmake")
        file:printf(build_dir .. "\n", "xmake")

        -- 关闭文件
        file:close()
    end

    os.execv("python ", {"scripts/tasks/make_sdk.py", "build/config.txt"})
    os.cp("$(projectdir)/scripts", "$(env SAMUI)")
end
