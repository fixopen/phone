main是主窗体。
其中LogoAndName是这个软件的名称，暂时没有定下来，可以先沿用商务录音电话【biz recorder？】
上面两个按钮分别为【voice messages】留言和【new call】来电，用形象的图代表，在有新留言或者来电的时候有所展现【闪烁？或者显示条数？】，当有内容时，点击分别进入【spec leaveWord type of phoneRecords】通话记录中的留言和【call】通话画面。
date and time区显示日期和时间
下面三个长条形的按钮【应该是大按钮好看点】分别是【contacts】联系人、【phone records】通话记录和【setting】设置。
点击分别进入contacts画面、phoneRecords画面和settingAll画面。

Contacts是联系人列表。
布局为上面一排操作按钮，左面是搜索区域，右边是联系人列表区域。
Close按钮应该放在最右上角用形象鲜明的X表示，其余几个按钮按现有顺序排列，它们分别是：【new category】新联系人类别、【new contact】新建联系人、【clear】删除所有联系人，【pageup】上一页、【pagedown】下一页。
搜索区域包括姓名【以及姓名缩写】、电话号码，联系人类别，联系人类型（黑名单、VIP之类的）这四项和一个Find按钮。
联系人列表区域是一个表格，其列分别是：【category】联系人类别，【name】名字，【telephone】电话，【address】地址，【type】类型，【features】操作。
其中category列下面对应的项点击后进入该category的编辑画面，features列下面对象的都是delete，点击后删除本条记录（行）。别的项点击后进入contact画面。
你布置一下，看看能放置多少行，同时，保证列的顺序不变。

contact是联系人详细信息显示和编辑画面。
最上面就是一个【close】X关闭按钮。
然后大致分两列展现联系人的详细信息，最下面的【remarks】备注比较大。
电话号码后面的call按钮点击以后进入call画面。

phoneRecords是通信历史记录列表。
其布置类同与联系人列表。上面一行为操作按钮区，左边为搜索区，右边为列表区。
其中【play message】点击以后会进入playRecords画面，该按钮放到clear按钮的左边以便于跟联系人列表的顺序一致。【close】自然还是放在最右上角了。
列表中列比较多，其中record列下面根据该条通话记录有没有录音显示不同的形状【比如：有录音显示一个音标符号，没有不显示等，我用has表示了一下】，有的话点击进入playRecords画面，features列下面有两项：call和delete，点击分别进入call画面和删除当前记录（行）。别的列下面的项点击无效。

playRecords是通信历史记录中的录音播放画面。
最右上角还是【close】X关闭按钮。
左边区域显示当前播放的录音相关的信息，包括姓名，号码，时间，时长，进度条，进度数据【比如2m3s/4m26s，表示已经播放了2分3秒，总共有4分26秒】，下面是【pause】暂停、【resume】继续、【delete】删除该条录音，其中暂停和继续可以放在一个位置，点击了暂停继续显示出来，点击了继续暂停显示出来。
右边区域显示音量控制和总体进度及控制，volume controller是一个音量控制器，【previous】前一个、【next】后一个用于控制在多个录音中切换，中间的文本框显示38/121之类的信息，表示总共有121条，当前为38条。

setting*为设置画面。
这四个画面是和在一起看的。SettingAll是一个架子，最上面一行包括【restore default setting】重置回默认设置和【close】关闭按钮。
下面是三个选择按钮：【telephone setting】电话设置、【system parameters】系统参数和【system info】系统信息。点击它们时，下面的空白区域分别显示settingForTelephone、settingForSystemParams和settingForSystemInfo。

settingForTelephone是电话设置的内容
包括【我按照顺序说】区号，外线前缀，IP前缀，默认振铃音，自动答录的振铃次数以及15个速拨设置。

settingForSystemParams是系统参数设置
包括录音数据的目录设置，振铃音量，系统音量，日期时间的校准，屏幕校准。

settingForSystemInfo是系统信息的展现
包括产品ID，版本号【硬件、软件】，系统空间【SD卡，去掉flash drive吧】，联系人数量，录音数量，通话记录数量，三个clear分别清空这三种信息。

category是联系人类别新建和编辑画面
包括名称输入，振铃，提示语的选择。play试听选中的振铃或者提示语。
delete删除该类别。
最右上角还是【close】X关闭。delete不要跟close在一行。

call是通话控制画面
同样，右上角还是【close】X关闭。
左上角【name】和【number】是联系人相关的信息，如果号码没有记入联系人【名片】中，【new contact】切换画面至新建联系人。如果已经记入名片中了，那么new contact按钮不显示。
左中显示通信信息。包括【communication status】通信状态：正在呼叫|对方忙|接通|对方挂断|……等等信息描述，同时【time and duration】给出接通的时间和通话的时长。
左下显示音量，录音信息和控制。通过音量控制器显示和修改当前音量，同时【mute】静音可以完全关闭音量。【record status[time and duration]】显示是否正在录音，如果是，显示录音的开始时间和时长，【start record】和【stop record】分别开始和停止录音，它们可以放在同一个位置，点击了开始停止显示出来，点击了停止开始显示出来。
右面显示一系列通话控制按钮。分别是：【reject】拒接，用于来电时；【listen】接听，用于来电时；【hangup】挂断，通话过程中任意时刻都可以点击；【hidden】隐藏，隐藏此画面，但是所有的状态都保持【也就是说该通话继续通话，该录音继续录音，只是该界面隐藏了】，通过点击main界面的【new call】按钮重新显示该画面。
