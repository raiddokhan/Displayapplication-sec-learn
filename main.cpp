// #include <stdio.h>
// #include "named_pipe.h"

// // int main()
// // {

// //     printf("yes");
// // }

// int main()
// {

//   int fd, fdR;
//   json message;
//   // FIFO file path
//   char *myfifo = "/tmp/myfifo1";
//   gen_mkfifo(myfifo, 0666);
//   // mkfifo(myfifo, 0666);
//   // Create two JSON objects
//   nlohmann::json j1;

//   std::string str = "test\\\nyes";
//   escape_escape_sequences(str);
//   std::cout << " " << str << std::endl;

//   while (true)
//   {
//     for (int i = 0; i < 100; i++)
//     {
//       //  std::cout << returnTotalVirtualMemory() << std::endl;
//       // if(i==0){
//       std::cout << "start with " << i << " print" << std::endl;
//       // }
//       j1["counter"] = i;

//       // Write the input arr2ing on FIFO
//       // and close it
//       write_message(fd, j1, myfifo);
//       std::cout << "message to print =>" << j1.dump() << std::endl;
//       unsigned int microsecond = 1000000;
//       usleep(microsecond);//sleeps for 3 second

//       // Read from FIFO
//     //   read_message(fdR, message, myfifo);
//     //   std::cout << " " << message.dump() << std::endl;
//     //   std::cout.flush();

//       // Print the read message
//       // printf("User2: %s\n", arr1);
//       // close(fd);
//     }
//     // unsigned int microsecond = 1000000;
//     // usleep(100);//sleeps for 3 second
//   }
//   // close(fd);
//   return 0;
// }

#include "config_loader/config.h"
#include "windows/rootwidget.h"
#include <QApplication>
#include <sys/stat.h>

#include <QFile>

//qwt test
// #include <qapplication.h>
// #include <qwt_plot.h>
// #include <qwt_plot_curve.h>
// #include <qwt_plot_grid.h>
// #include <qwt_symbol.h>
// #include <qwt_legend.h>


ConfigLoader configLoaded(2);

inline bool exists_test (const std::string& name) {
  struct stat buffer;
  return (stat (name.c_str(), &buffer) == 0);
}

inline bool initConfig() {
    configLoaded.loadNewConfig("/home/mannigel/Projects/Sec/config/ball_order_config.json",0);
    configLoaded.loadNewConfig("/home/mannigel/Projects/Sec//config/base_config.json",1);
    // std::cout << configLoaded[0].second.dump() << std::endl;
    return true;
}

int main(int argc, char *argv[]) 
{
    initConfig();
    QApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    // Set the app style sheetg
    QFile styleSheetFile("/home/mannigel/qt_code/MainWindow/style/Geoo.qss");
    if(!exists_test("/home/mannigel/qt_code/MainWindow/style/Geoo.qss")){
        printf("file dose not exits\n");
    }

    if(styleSheetFile.open(QIODevice::ReadOnly)) {
        QString styleSheet = QLatin1String(styleSheetFile.readAll());
        a.setStyleSheet(styleSheet);
    } else {
        printf("Error_no_Style");
    }


    RootWidget w;
    w.setWindowState(Qt::WindowFullScreen);
    w.show();
    
    json tmp = configLoaded[1].second["ui_elements"]["Dial"]["maximum"];
    w.changeProfileDialChanger(tmp["min"],tmp["max"],tmp["steps"]);
    w.updateLCDNumber(2);

    return a.exec();
    
}

