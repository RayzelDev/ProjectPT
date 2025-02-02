using System;
using System.IO;
using System.Net;
using System.Linq;
using System.Windows;
using GCLauncher.Lang;
using GCLauncher.Source;
using System.Threading.Tasks;
using System.Windows.Controls;
using System.Security.Cryptography;

namespace GCLauncher{
    public partial class Settings : Window{
        public static Info DllInfo = new Info();
        public static Language settingsLang = new Language();
        public bool settingDebug = false;
        public Settings(bool debugInfo){
            settingDebug = debugInfo;
            InitializeComponent();
            SettingsStart();
            Show();
        }
        public void SettingsStart(){
            LangText.Text = settingsLang.LangString(4);
            ButtonResolve.Text = settingsLang.LangString(12);
            var os = Environment.OSVersion;
            if(os.Platform == PlatformID.Win32NT && os.Version.Major >= 6){
                Button_Text_1.IsEnabled = true;
                DllBarTotal.Value = 0;
            }
            switch(settingsLang.LangDefined()){
                case "0": br_lang.IsChecked = true; break;
                case "1": us_lang.IsChecked = true; break;
                case "2": kr_lang.IsChecked = true; break;
                case "3": es_lang.IsChecked = true; break;
            }
        }
        private int GetDllDownload(){
            if(!File.Exists("d3d9_.dll")){
                return 1;
            }
            int dllID = 0;
            DllInfo.DllPopulate();
            byte[] buff = File.ReadAllBytes(@"d3d9.dll");
            var md5 = MD5.Create().ComputeHash(buff);
            string hash = BitConverter.ToString(md5).Replace("-", "").ToLowerInvariant();
            foreach (var dllUnit in DllInfo.Win10Info){
                if(hash == dllUnit.DllHash){
                    dllID = dllUnit.DllID + 1;
                    break;
                }
            }
            if(dllID == 0 || dllID > DllInfo.Win10Info.Last().DllID){
                return 1;
            }
            return dllID;
        }
        private void DllProgressChanged(object sender, DownloadProgressChangedEventArgs e){
            DllBarTotal.Value = e.ProgressPercentage;
        }
        private async void DllDownloading(){
            int DllDownload = GetDllDownload();
            string output = null;
            cls.IsEnabled = false;
            Button_Text_1.IsEnabled = false;
            using (var client = new WebClient()){
                client.DownloadFileCompleted += (sender, e) =>{
                    output = e.ToString();
                };
                client.DownloadProgressChanged += new DownloadProgressChangedEventHandler(DllProgressChanged);
                client.DownloadFileAsync(new Uri(DllInfo.launcherURL + "/Dll/" + DllDownload + ".dll"), "d3d9.dll");
                var n = DateTime.Now;
                while(output == null){
                    await Task.Delay(1);
                }
            }
            cls.IsEnabled = true;
            Button_Text_1.IsEnabled = true;
        }
        private void ClickSettings(object sender, RoutedEventArgs e){
            string content = (sender as Button).Name.ToString();
            switch(content){
                case "Button_Text_1": DllDownloading(); break;
                case          "clbs":          Close(); break;
            }
        }
        private void ChangeLanguage(object sender, RoutedEventArgs e){
            var button = sender as RadioButton;
            switch(button.Name){
                case "br_lang": settingsLang.LangSet("0"); break;
                case "us_lang": settingsLang.LangSet("1"); break;
                case "kr_lang": settingsLang.LangSet("2"); break;
                case "es_lang": settingsLang.LangSet("3"); break;
            }
            SettingsStart();
            foreach(Window window in Application.Current.Windows){
                bool settingsMaintenance;
                if(!settingDebug){
                    settingsMaintenance = bool.Parse(DllInfo.GetJson(1));
                }
                else{
                    settingsMaintenance = true;
                }
                if(window.GetType() == typeof(Launcher)){
                    if((window as Launcher).totalText.Text == "100%"){
                        (window as Launcher).fileText.Text = settingsLang.LangString(9);
                    }
                    if(!settingsMaintenance){
                        (window as Launcher).serverText.Text = settingsLang.LangString(2);
                    }
                    else{
                        (window as Launcher).serverText.Text = settingsLang.LangString(3);
                    }
                    (window as Launcher).LauncherChangeInit();
                }
            }
        }
    }
}