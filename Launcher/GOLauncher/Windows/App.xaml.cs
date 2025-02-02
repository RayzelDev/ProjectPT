using System;
using System.Diagnostics;
using System.Windows;
using GCLauncher.Source;

namespace GCLauncher{
    public partial class App : Application{
        string stringData;
        public bool debugMode = false;
        public static Info statusApp = new Info();
        void AppMain(object sender, StartupEventArgs e){
            stringData = statusApp.GetFileString(2);
            if(stringData == null){
                MessageBox.Show("Tente novamente mais tarde!", "Sem conexão!");
                Environment.Exit(0);
            }
            else{

                // Admin
                if(!statusApp.IsAdministrator()){
                    MessageBox.Show("Execute o launcher como administrador!", "Erro de permissão!");
                    Environment.Exit(0);
                }

                // Main exe aberto
                if (Process.GetProcessesByName("main").Length != 0) {
                    MessageBox.Show("Main.exe está aberto por favor finalize-o.", "Erro");
                    Environment.Exit(0);
                }

                // Normal
                Splash Load = new Splash();
            }
        }
    }
}