<html>
<body>
<pre>
<h1>Build Log</h1>
<h3>
--------------------Configuration: MultimediaPhone - Win32 (WCE MIPSII) Debug--------------------
</h3>
<h3>Command Lines</h3>
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP142.tmp" with contents
[
/nologo /W3 /GX /Zi /Od /D "DEBUG" /D "MIPS" /D "_MIPS_" /D "R4000" /D "MIPSII" /D UNDER_CE=500 /D _WIN32_WCE=500 /D "WCE_PLATFORM_ingenic" /D "UNICODE" /D "_UNICODE" /D "_AFXDLL" /FR"MIPSIIDbg/" /Fp"MIPSIIDbg/MultimediaPhone.pch" /Yu"stdafx.h" /Fo"MIPSIIDbg/" /Fd"MIPSIIDbg/" /QMmips2 /QMFPE /MC /c 
"D:\lxz_work_dir\media_phone\new_sources\View\contactgroupdlg.cpp"
]
Creating command line "clmips.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP142.tmp" 
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP143.tmp" with contents
[
lib\\sqlite.lib ws2.lib mmtimer.lib lib\\csplayer.lib cximage.lib lib\\jpeg.lib lib\\png.lib lib\\zlib.lib commctrl.lib Coredll.lib lib\\lamelib.lib lib\\mpglib.lib lib\\libshine.lib /nologo /base:"0x00010000" /stack:0x10000,0x1000 /entry:"wWinMainCRTStartup" /incremental:yes /pdb:"MIPSIIDbg/MultimediaPhone.pdb" /debug /out:"MIPSIIDbg/MultimediaPhone.exe" /libpath:"E:\My Documents\My Projects\MultimediaPhone\lib" /subsystem:windowsce,5.00 /MACHINE:MIPS 
.\MIPSIIDbg\MultimediaPhone.obj
.\MIPSIIDbg\MultimediaPhoneDlg.obj
.\MIPSIIDbg\StdAfx.obj
.\MIPSIIDbg\Contact.obj
.\MIPSIIDbg\ContactData.obj
.\MIPSIIDbg\ContactGroup.obj
.\MIPSIIDbg\ContactInfo.obj
.\MIPSIIDbg\LanguageResource.obj
.\MIPSIIDbg\log.obj
.\MIPSIIDbg\Picture.obj
.\MIPSIIDbg\SchedulerData.obj
.\MIPSIIDbg\Setting.obj
.\MIPSIIDbg\SkinStyle.obj
.\MIPSIIDbg\SoundSegment.obj
.\MIPSIIDbg\TelephoneNumber.obj
.\MIPSIIDbg\User.obj
.\MIPSIIDbg\CalculaterDlg.obj
.\MIPSIIDbg\ContactDlg.obj
.\MIPSIIDbg\contactgroupdlg.obj
.\MIPSIIDbg\ContactInfoDlg.obj
.\MIPSIIDbg\ContactInfoSearchDlg.obj
.\MIPSIIDbg\ContactNewDlg.obj
.\MIPSIIDbg\contactsearchdlg.obj
.\MIPSIIDbg\deletetipdlg.obj
.\MIPSIIDbg\fastdialsdlg.obj
.\MIPSIIDbg\firewalldlg.obj
.\MIPSIIDbg\InformationDlg.obj
.\MIPSIIDbg\LunarderDlg.obj
.\MIPSIIDbg\MainDlg.obj
.\MIPSIIDbg\MainMenuDlg.obj
.\MIPSIIDbg\MainMp3Dlg.obj
.\MIPSIIDbg\MainPhotoDlg.obj
.\MIPSIIDbg\MainVideoDlg.obj
.\MIPSIIDbg\notedlg.obj
.\MIPSIIDbg\passworddlg.obj
.\MIPSIIDbg\Player.obj
.\MIPSIIDbg\PlayerDlg.obj
.\MIPSIIDbg\playsounddlg.obj
.\MIPSIIDbg\screensavedlg.obj
.\MIPSIIDbg\SettingDlg.obj
.\MIPSIIDbg\SoundDlg.obj
.\MIPSIIDbg\soundsearchdlg.obj
.\MIPSIIDbg\telephonedlg.obj
.\MIPSIIDbg\fsm.obj
.\MIPSIIDbg\socket.ce.obj
.\MIPSIIDbg\socket.obj
.\MIPSIIDbg\stringOp.obj
.\MIPSIIDbg\thread.obj
.\MIPSIIDbg\timer.obj
.\MIPSIIDbg\LunarderDate.obj
.\MIPSIIDbg\Stock.obj
.\MIPSIIDbg\Telephone.obj
.\MIPSIIDbg\CeBtnST.obj
.\MIPSIIDbg\CEComboBox_.obj
.\MIPSIIDbg\CEDialog.obj
.\MIPSIIDbg\CELineEdit.obj
.\MIPSIIDbg\CEListCtrl.obj
.\MIPSIIDbg\CELunarder.obj
.\MIPSIIDbg\CEProcessBar.obj
.\MIPSIIDbg\CERadioButton.obj
.\MIPSIIDbg\CEScrollBar_.obj
.\MIPSIIDbg\CEStatic.obj
.\MIPSIIDbg\DIBSectionCE.obj
.\MIPSIIDbg\InputDlg.obj
.\MIPSIIDbg\SoftKey.obj
.\MIPSIIDbg\MultimediaPhone.res
]
Creating command line "link.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP143.tmp"
<h3>Output Window</h3>
Compiling...
contactgroupdlg.cpp
Linking...
   Creating library MIPSIIDbg/MultimediaPhone.lib and object MIPSIIDbg/MultimediaPhone.exp
sqlite.lib(alter.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(analyze.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(attach.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(auth.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(btree.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(build.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(callback.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(complete.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(date.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(delete.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(expr.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(func.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(hash.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(insert.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(legacy.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(loadext.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(main.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(malloc.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(opcodes.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(os.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(os_win.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(pager.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(parse.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(pragma.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(prepare.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(printf.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(random.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(select.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(table.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(tokenize.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(trigger.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(update.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(utf.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(util.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(vacuum.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(vdbe.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(vdbeapi.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(vdbeaux.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(vdbefifo.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(vdbemem.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(vtab.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
sqlite.lib(where.obj) : warning LNK4204: 'D:\lxz_work_dir\media_phone\new_sources\MIPSIIDbg\vc60.pdb' is missing debugging information for referencing module; linking object as if no debug info
corelibc.lib(wwinmain.obj) : warning LNK4209: debugging information corrupt; recompile module; linking object as if no debug info
corelibc.lib(onexit.obj) : warning LNK4209: debugging information corrupt; recompile module; linking object as if no debug info
corelibc.lib(crt0dat.obj) : warning LNK4209: debugging information corrupt; recompile module; linking object as if no debug info
corelibc.lib(crt0init.obj) : warning LNK4209: debugging information corrupt; recompile module; linking object as if no debug info
Strmiids.lib(strmguid.obj) : warning LNK4209: debugging information corrupt; recompile module; linking object as if no debug info
Strmiids.lib(strmif_i.obj) : warning LNK4209: debugging information corrupt; recompile module; linking object as if no debug info
Creating temporary file "C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP149.tmp" with contents
[
/nologo /o"MIPSIIDbg/MultimediaPhone.bsc" 
.\MIPSIIDbg\StdAfx.sbr
.\MIPSIIDbg\MultimediaPhone.sbr
.\MIPSIIDbg\MultimediaPhoneDlg.sbr
.\MIPSIIDbg\Contact.sbr
.\MIPSIIDbg\ContactData.sbr
.\MIPSIIDbg\ContactGroup.sbr
.\MIPSIIDbg\ContactInfo.sbr
.\MIPSIIDbg\LanguageResource.sbr
.\MIPSIIDbg\log.sbr
.\MIPSIIDbg\Picture.sbr
.\MIPSIIDbg\SchedulerData.sbr
.\MIPSIIDbg\Setting.sbr
.\MIPSIIDbg\SkinStyle.sbr
.\MIPSIIDbg\SoundSegment.sbr
.\MIPSIIDbg\TelephoneNumber.sbr
.\MIPSIIDbg\User.sbr
.\MIPSIIDbg\CalculaterDlg.sbr
.\MIPSIIDbg\ContactDlg.sbr
.\MIPSIIDbg\contactgroupdlg.sbr
.\MIPSIIDbg\ContactInfoDlg.sbr
.\MIPSIIDbg\ContactInfoSearchDlg.sbr
.\MIPSIIDbg\ContactNewDlg.sbr
.\MIPSIIDbg\contactsearchdlg.sbr
.\MIPSIIDbg\deletetipdlg.sbr
.\MIPSIIDbg\fastdialsdlg.sbr
.\MIPSIIDbg\firewalldlg.sbr
.\MIPSIIDbg\InformationDlg.sbr
.\MIPSIIDbg\LunarderDlg.sbr
.\MIPSIIDbg\MainDlg.sbr
.\MIPSIIDbg\MainMenuDlg.sbr
.\MIPSIIDbg\MainMp3Dlg.sbr
.\MIPSIIDbg\MainPhotoDlg.sbr
.\MIPSIIDbg\MainVideoDlg.sbr
.\MIPSIIDbg\notedlg.sbr
.\MIPSIIDbg\passworddlg.sbr
.\MIPSIIDbg\Player.sbr
.\MIPSIIDbg\PlayerDlg.sbr
.\MIPSIIDbg\playsounddlg.sbr
.\MIPSIIDbg\screensavedlg.sbr
.\MIPSIIDbg\SettingDlg.sbr
.\MIPSIIDbg\SoundDlg.sbr
.\MIPSIIDbg\soundsearchdlg.sbr
.\MIPSIIDbg\telephonedlg.sbr
.\MIPSIIDbg\fsm.sbr
.\MIPSIIDbg\socket.ce.sbr
.\MIPSIIDbg\socket.sbr
.\MIPSIIDbg\stringOp.sbr
.\MIPSIIDbg\thread.sbr
.\MIPSIIDbg\timer.sbr
.\MIPSIIDbg\LunarderDate.sbr
.\MIPSIIDbg\Stock.sbr
.\MIPSIIDbg\Telephone.sbr
.\MIPSIIDbg\CeBtnST.sbr
.\MIPSIIDbg\CEComboBox_.sbr
.\MIPSIIDbg\CEDialog.sbr
.\MIPSIIDbg\CELineEdit.sbr
.\MIPSIIDbg\CEListCtrl.sbr
.\MIPSIIDbg\CELunarder.sbr
.\MIPSIIDbg\CEProcessBar.sbr
.\MIPSIIDbg\CERadioButton.sbr
.\MIPSIIDbg\CEScrollBar_.sbr
.\MIPSIIDbg\CEStatic.sbr
.\MIPSIIDbg\DIBSectionCE.sbr
.\MIPSIIDbg\InputDlg.sbr
.\MIPSIIDbg\SoftKey.sbr]
Creating command line "bscmake.exe @C:\DOCUME~1\ADMINI~1\LOCALS~1\Temp\RSP149.tmp"
Creating browse info file...
<h3>Output Window</h3>




<h3>Results</h3>
MultimediaPhone.exe - 0 error(s), 48 warning(s)
</pre>
</body>
</html>
