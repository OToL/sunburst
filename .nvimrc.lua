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

local adventrun = Terminal:new({
    cmd = "sb_advent_2022.exe",
    dir = "build/win64-ninja-clang/samples/sb_advent_2022",
    hidden = true,
    close_on_exit = false,
    direction = "float"
})
function sbRunAdvent()
    --adventrun:toggle()
    vim.api.nvim_exec("!start cmd  /k \"mode con: cols=180 lines=80 && cd build/win64-ninja-clang/samples/sb_advent_2022 && sb_advent_2022.exe && pause && exit\"", true)
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
    elseif (string.find(buffer_name, "sb_advent_2022")) then
        sbRunAdvent()
    else
        sbRunUnitTests()
    end
end

-- Lua mapping example
vim.api.nvim_set_keymap("n", "<F5>", "<cmd>lua sbRun(vim.api.nvim_buf_get_name(0))<CR>", {noremap = true, silent = true})

