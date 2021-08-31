function main(...)
    -- https://xmake.io/#/zh-cn/manual/plugin_task
    import("core.base.task")
    task.run("mksdk")
    task.run("insdk")
end
