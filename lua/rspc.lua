RSPC = {}

function RSPC.gen_ccjson(dir, src)
  local outf = dir .. "/compile_commands.json"
  print(outf)
  local fout = io.open(outf, 'w')
  local fmt = [=[
[
  {
    "arguments": [
      "/usr/bin/g++",
      "-c",
      "-std=c++17",
      "-g",
      "-Wall",
      "-DRSPC_TRACE_HINT=1",
      "-I/usr/include/x86_64-linux-gnu/c++/11",
      "-I/usr/lib/gcc/x86_64-linux-gnu/11/include",
      "-I/usr/include/x86_64-linux-gnu",
      "-I/usr/include/c++/11",
      "-I/usr/include/c++/11/backward",
      "-DRSPC_TRACE_BTIME=\"08/05/23-22h-37m-35s\"",
      "-o",
      "./main",
      "main.cc"
    ],
    "directory": "%s",
    "file": "%s",
    "output": "%s"
  }
]
  ]=]
  fout:write(string.format(fmt, dir, src, (dir .. "/main")))
  fout:close()
end
