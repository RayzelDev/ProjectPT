using System;
using System.IO;
using System.Net;
using System.Diagnostics;
using System.Windows.Controls;
using System.Xml.Serialization;
using System.Collections.Generic;
using System.Security.Cryptography;
using System.Threading.Tasks;
using System.Text;
using System.Security.Principal;
using System.Windows;

namespace GCLauncher.Source{
    public class Info{
        public string launcherExec = Process.GetCurrentProcess().ProcessName;
        public string launcherURL = "http://191.96.78.143:90/Launcher/";
        private readonly string launcherParam = "VAI_TOMAR_NO_CU_CURIOSO_KKKKKK";
        public List<Arquivo> FileInfo { get; set; }
        public string GetFileString(int type){
            string getString = null;
            switch(type){
                case 0: getString = launcherURL + "/files.xml"; break;
                case 1: getString = launcherURL + "/maintenance.txt"; break;
                case 2: getString = launcherURL + "/launcher.txt"; break;
            }
            try{
                using(var client = new WebClient()){
                    return client.DownloadString(getString);
                }
            }
            catch{
                return null;
            }
        }
        public void StartMain(){
            ProcessStartInfo startInfo = new ProcessStartInfo("Game.exe")
            {
                Arguments = launcherParam
            };
            Process.Start(startInfo);
        }
        public string FileData(string path, int opt){
            if(opt == 0){
                return Path.GetFileName(path);
            }
            if(opt == 1){
                return Path.GetDirectoryName(path);
            }
            path.Replace('\'', '/');
            return path;
        }
        public string GetChecksum(string path){
            using (var stream = new BufferedStream(File.OpenRead(path), 32768))
            {
                MD5CryptoServiceProvider md5 = new MD5CryptoServiceProvider();
                byte[] checksum = md5.ComputeHash(stream);
                return BitConverter.ToString(checksum).Replace("-", String.Empty).ToLower();
            }
        }
        public Arquivo FileCheckUpdate(Arquivo unit, int total, ref ProgressBar LauncherBarTotal, ref TextBlock splashText){
            int launcherPercent = (100 * unit.FileID) / total;
            LauncherBarTotal.Value = launcherPercent;
            splashText.Text = "Verificando arquivos " + launcherPercent + "%";
            if (!File.Exists(@unit.FilePath)){
                unit.ToUpdate = true;
            }
            else{
                if(GetChecksum(unit.FilePath) != unit.FileHash){
                    unit.ToUpdate = true;
                }
            }
            return unit;
        }
        public void FilePopulate(){
            string xmlString = GetFileString(0);
            XmlSerializer serializer  = new XmlSerializer(typeof(List<Arquivo>), new XmlRootAttribute("ArrayOfArquivo"));
            StringReader stringReader = new StringReader(xmlString);
            FileInfo = (List<Arquivo>)serializer.Deserialize(stringReader);
        }
        public bool IsAdministrator(){
            return (new WindowsPrincipal(WindowsIdentity.GetCurrent())).IsInRole(WindowsBuiltInRole.Administrator);
        }
    }
}