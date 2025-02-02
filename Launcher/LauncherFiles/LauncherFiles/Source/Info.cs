using System;
using System.IO;
using System.Xml.Serialization;
using System.Collections.Generic;
using System.Security.Cryptography;

namespace LauncherFiles.Source{
    public class Info{
        string FileDir = AppDomain.CurrentDomain.BaseDirectory;
        List<Arquivo> FileA = new List<Arquivo>();
        public static byte[] ConverteStreamToByteArray(Stream stream){
            byte[] byteArray = new byte[16 * 1024];
            using(MemoryStream mStream = new MemoryStream()){
                int bit;
                while((bit = stream.Read(byteArray, 0, byteArray.Length)) > 0){
                    mStream.Write(byteArray, 0, bit);
                }
                return mStream.ToArray();
            }
        }
        public string GetChecksum(Stream input){
            using (var stream = new BufferedStream(input, 32768))
            {
                MD5CryptoServiceProvider md5 = new MD5CryptoServiceProvider();
                byte[] checksum = md5.ComputeHash(stream);
                return BitConverter.ToString(checksum).Replace("-", String.Empty).ToLower();
            }
        }
        public void FileListCreate(){
            int FileID = 1;
            string[] allfiles = Directory.GetFiles(FileDir, "*.*", SearchOption.AllDirectories);
            foreach(var file in allfiles){
                FileInfo info = new FileInfo(file);
                if(info.Name != "LauncherFiles.exe" && info.Name != "grandchase.dll" && info.Name != "grandchase.exe"){
                    string hashFinal;
                    using (FileStream fs = File.OpenRead(@info.DirectoryName + "\\" + info.Name)){
                        hashFinal = GetChecksum(fs);
                    }
                    Uri TempPath = new Uri(info.Directory + "\\" + info.Name);
                    Uri TempFolder = new Uri(FileDir);
                    string RelativePath = Uri.UnescapeDataString(TempFolder.MakeRelativeUri(TempPath).ToString().Replace('/', Path.DirectorySeparatorChar));
                    FileA.Add(new Arquivo(){
                        FileID = FileID,
                        FilePath = RelativePath,
                        FileHash = hashFinal
                    });
                    Console.WriteLine("FileID: " + FileID + " - FilePath: " + RelativePath + " - Hash: " + hashFinal);
                    FileID++;
                }
            }
        }
        public void FileListWrite(){
            XmlSerializer ser = new XmlSerializer(typeof(List<Arquivo>));
            using(FileStream fs = new FileStream(@"files.xml", FileMode.Create)){
                ser.Serialize(fs, FileA);
            }
            Console.WriteLine("File List was written on file!");
        }
    }
    public class Arquivo{
        public int FileID { get; set; }
        public string FileHash { get; set; }
        public string FilePath { get; set; }
    }
}