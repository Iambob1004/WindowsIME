using System;
using System.ComponentModel;
using System.Configuration.Install;
using System.Runtime.InteropServices;

[RunInstaller(true)]
public class CustomInstaller : Installer
{
    public override void Install(System.Collections.IDictionary stateSaver)
    {
        base.Install(stateSaver);
        string targetDir = Context.Parameters["targetdir"];
        string dllPath= Path.Combine(targetDir, "SampleIME.dll");
        InstallLayoutOrTip(dllPath, 0);  
    }


}