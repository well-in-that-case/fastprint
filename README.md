# fastprint
Same behavior as the normal print function, just upwards of 6x faster.

### Do I need to change anything in my code?
No. The behavior between `fastprint` and `print` in Lua >= 5.2 is identical.

### Example
```lua
local fastprint = require "fastprint"
local print = fastprint.print
```
