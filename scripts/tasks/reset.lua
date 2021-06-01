function main(...)
    -- https://xmake.io/#/zh-cn/manual/plugin_task
    if os.exists(".xmake") then
        os.rmdir(".xmake")
    end

    if os.exists("build") then
        os.rmdir("build")
    end
end
