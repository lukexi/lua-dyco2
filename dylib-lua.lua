
local debug_flags = "-fno-omit-frame-pointer -g -Wall"
local perf_flags  = "-O2"
local include_flags  = "-I../realtalk/src -I../rtlua/src -I../rtlua/vendor/lua"
local lib_flags = "-fPIC -shared -Wl,-undefined,dynamic_lookup"
math.randomseed(os.time())
function compile_source(source)
    local name = tostring(math.random(0,1000000)) -- this should be a hash of the source
    local source_filename = "/tmp/"..name..".c"
    local lib_filename    = "/tmp/"..name..".so"

    local source_file_handle = io.open(source_filename, "w")
    source_file_handle:write(source)
    source_file_handle:close()

    local args =
        "clang"
        .. " " .. source_filename
        .. " " .. "-o " .. lib_filename
        .. " " .. lib_flags
        .. " " .. include_flags
        .. " " .. debug_flags
        .. " " .. perf_flags
        .. " " .. "2>&1"
    local proc = io.popen(args)
    local output = ""
    for line in proc:lines() do
       output = output .. "\n" .. line
    end
    proc:close()

    if #output > 0 then
        print(output)
    end

    return lib_filename
end
