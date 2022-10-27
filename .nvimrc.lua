vim.cmd([[
    set errorformat^=%-GIn\ file\ included\ %.%#
    set makeprg=ninja\ -C\ build/win64-ninja-clang/]])

local Terminal = require("toggleterm.terminal").Terminal

local cmakegen = Terminal:new({
    cmd = "gen_win64_ninja_clang.bat",
    dir = "script/build",
    hidden = true,
    close_on_exit = false,
    direction = "float"
})
function sbCMakeGen()
    cmakegen:toggle()
end

local unittestrun = Terminal:new({
    cmd = "sb_core_test.exe",
    dir = "build/win64-ninja-clang/engine/test/sb_core_test",
    hidden = true,
    close_on_exit = false,
    direction = "float"
})
function sbRunUnitTests()
    print("hello world")
    unittestrun:toggle()
end

local playgroundrun = Terminal:new({
    cmd = "sb_playground.exe",
    dir = "build/win64-ninja-clang/samples/sb_playground",
    hidden = true,
    close_on_exit = true,
    direction = "float"
})
function sbRunPlayground()
    playgroundrun:toggle()
end

local gitstatus = Terminal:new({
    cmd = "git status",
    hidden = true,
    close_on_exit = false,
    direction = "float"
})
function sbGitStatus()
    gitstatus:toggle()
end

function sbRun(buffer_name)
    if (string.find(buffer_name, "sb_playground")) then
        sbRunPlayground()
    else
        sbRunUnitTests()
    end
end

-- Lua mapping example
vim.api.nvim_set_keymap("n", "<C-k>r", "<cmd>lua sbRun(vim.api.nvim_buf_get_name(0))<CR>", {noremap = true, silent = true})
