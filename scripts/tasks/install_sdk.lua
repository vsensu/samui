function main(...)
    -- https://xmake.io/#/zh-cn/manual/plugin_task
    local target_dir = "$(env SAMUI)"
    print("[install] install samui ...")
    os.cp("$(projectdir)/build/sdk/*", target_dir)
end