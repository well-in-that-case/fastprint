local fastprint = require "fastprint"
local new_print = fastprint.print
local old_print = print

local function benchmark(callback, iterations, ...)
    local i = 0
    local t1 = os.clock()
    local ret
    repeat
        ret = callback(...)
        i = i + 1
    until i == iterations
    local t2 = os.clock()

    return t2 - t1, ret
end

local time = benchmark(new_print, 10000, "hello world")
local time0 = benchmark(old_print, 10000, "hello world")

local time1 = benchmark(new_print, 10000, "hello world", 1, nil, {}, "arg5", nil, 5, {} )
local time2 = benchmark(old_print, 10000, "hello world", 1, nil, {}, "arg5", nil, 5, {} )

new_print "Simple argument run-time expenses:"
new_print(time)
new_print(time0)

new_print "Dynamic argument run-time expenses:"
new_print(time1)
new_print(time2)
