function main(...)
    -- https://xmake.io/#/zh-cn/manual/plugin_task
    import("core.base.task")
    task.run("clean")

    if os.exists("build") then
        os.rmdir("build")
        os.mkdir("build")
    end
end
