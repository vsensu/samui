-- 导入配置模块
import("core.project.config")

-- 导入工程模块
import("core.project.project")

import("core.base.json")

function xmenv(name)
    local temp_path = path.join("$(projectdir)", "build" , "__xmenv__")
    -- 打开文件：w 为写模式, a 为追加写模式
    local file = io.open(temp_path, "w")
    if file then
        -- 使用xmake扩展的格式化传参写入一行，无换行符，并且支持内置变量
        file:printf(name, "xmake")

        -- 关闭文件
        file:close()
    end

    -- read and return
    local data = io.readfile(temp_path)
    os.rm(temp_path)
    return data
end

function get_version_str(version)
    local str = version.major .. "." .. version.minor .. "." .. version.patch
    if version.alpha ~= 0 then
        str = str .. "-alpha"
    end
    return str
end

function makesdk(conf)
    -- 生成各个版本
    -- 拷贝头文件
    -- 拷贝各个版本的lib
    -- 拷贝资源
    -- 拷贝模板
    -- 拷贝脚本
 
    local config_path = path.join(conf.project_dir, "build" , "config.json")

    for i, plat in pairs(conf.platforms) do
        for j, arch in pairs(conf.archs) do
            for k, mode in pairs(conf.modes) do
                local xmake_config_cmd = "xmake f -p " .. plat .. " -a " .. arch .. " -m " .. mode
                os.exec(xmake_config_cmd)

                local xmake_build_cmd = "xmake -r samui-engine"
                os.exec(xmake_build_cmd)
                print(xmake_config_cmd .. " rebuild finished!")
            end
        end
    end

    io.writefile(config_path, json.encode(conf))

    os.execv("python ", {"scripts/tasks/make_sdk.py", config_path})
    os.cp("$(projectdir)/scripts", path.join(conf.build_dir, "scripts"))
end

function main(...)
    -- https://xmake.io/#/zh-cn/manual/plugin_task

    -- 先加载工程配置
    config.load()

    local project_dir = "$(projectdir)"
    local build_dir = ""

    local target = project.target("samui-engine")
    local version = {
        major = 0,
        minor = 0,
        patch = 0,
        alpha = 0
    }
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

        version.major = target:values("samui.version.major")
        version.minor = target:values("samui.version.minor")
        version.patch = target:values("samui.version.patch")
        version.alpha = target:values("samui.version.alpha")

        print("major:" .. version.major)
        print("minor:" .. version.minor)
        print("patch:" .. version.patch)
        print("alpha:" .. version.alpha)

        -- build_dir = path.join(target:targetdir())
        build_dir = path.join(xmenv("$(projectdir)"), "build/sdk/", get_version_str(version))
        print("sdk build dir:" .. build_dir)
    end

    sdk_config = {
        version = version,
        project_dir = xmenv("$(projectdir)"),
        build_dir = build_dir,
        sdk_dir = xmenv("$(env SAMUI)"),
        
        platforms = {
            config.get("plat"),
        },

        archs = {
            "x86",
            "x64"
        },
        
        modes = {
            "debug",
            "release",
        },
    }

    makesdk(sdk_config)
end
