### Sublime 安装包配置：

Ctrl+Shift+P:

Package Control，输入ip，安装 install packages

### 可选包：

* ChineseLocalizations
    - 中文汉化包
* ConvertToUTF8
    - UTF8编码包
* DocBlocker 
    - 自动注释生成包
* Side Bar Enhancements
    - 侧面文件系统包（关键）  
* BracketHighlighter
    - 括号高亮包
* SublimeCodeIntel
    - 代码格式包（关键）
* Material Theme
    - 主题风格包
* AutoFileName
    - 自动文件名包
* All AutoComplete
    - 跨文件补齐包


### Markdown 专用包：

* MarkdownEditing
    - **必备**
* MarkdownPreview
    - **必备**安装后：
    - Ctrl+Shift+P : 输入mdp找到并选中`Markdown Preview： Preview in Browser` ,选择markerdown
    - 在Preferences -> Key Bindings打开的文件的右侧栏的中括号中添加一行代码：`{ "keys": ["ctrl+shift+q"], "command": "markdown_preview", "args": {"target": "browser", "parser":"markdown"}  }`
* LiveReload 
    - 自动刷新预览：
    - Preferences -> Package Settings -> Markdown Preview -> Settings，检查左侧enable_autoreload条目是否为true
    - Ctrl+shift+p, 输入LiveReload: Enable/disable plug-ins, 回车, 选择 Simple Reload with delay (400ms)或者Simple Reload


### Remove:

Ctrl+Shift+P: remove.
