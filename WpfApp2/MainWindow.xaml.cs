using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Windows;
using System.Windows.Controls;
using System.Windows.Data;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using System.Windows.Shapes;
using System.Threading;
using MsgPassingCommunication;
using System.IO;

namespace WpfApp2
{
  public partial class MainWindow : Window
  {
    public MainWindow()
    {
      InitializeComponent();
    }

    private bool isConnect = false;

    private Stack<string> pathStack_ = new Stack<string>();
    private Translater translater;
    private CsEndPoint endPoint_;
    private Thread rcvThrd = null;
    private Dictionary<string, Action<CsMessage>> dispatcher_
      = new Dictionary<string, Action<CsMessage>>();

    //----< show file text >----------------
    private void showFile(string fileName, string fileContents)
    {
      Paragraph paragraph = new Paragraph();
      paragraph.Inlines.Add(new Run(fileContents));
      CodePopUpWindow popUp = new CodePopUpWindow();
      popUp.Show();
      popUp.codeView.Blocks.Clear();
      popUp.codeView.Blocks.Add(paragraph);
    }

    //----< process incoming messages on child thread >----------------

    private void processMessages()
    {
      ThreadStart thrdProc = () => {
        while (true)
        {
          CsMessage msg = translater.getMessage();
          //if(!msg.attributes.ContainsKey("command"))
          //{
          //  continue;
          //}
          Console.WriteLine("\n  getting message in Translater");
          msg.show();
          string msgId = msg.value("command");
          if (dispatcher_.ContainsKey(msgId))
            dispatcher_[msgId].Invoke(msg);
        }
      };
      rcvThrd = new Thread(thrdProc);
      rcvThrd.IsBackground = true;
      rcvThrd.Start();
    }

    //----< process tests on child thread >----------------

    private void process()
    {
      ThreadStart thrdProc = () => {
        Console.WriteLine("\n  Demonstrating Requirement #0");
        Console.WriteLine("\n  using Visual Studio 2017 and the standard C++ libraries. \n  also using C#, the .Net Windows Presentation Foundation framework, \n  and C++\\CLI for the graphical part of each Client.");
        Console.WriteLine("\n  Demonstrating Requirement #1 - Connecting to Servers");
        test1();          // test for connect button
        test2();          // test for check-in button
        test3();          // test for check-out button
        test4();          // test for browse & view-file button
        test5();          // test for view-metadata button
      };
      Thread Thrd = new Thread(thrdProc);
      Thrd.IsBackground = true;
      Thrd.Start();
    }

    //----< function dispatched by child thread to main thread >-------

    private void clearStatus()
    {
      StatusBarText.Items.Clear();
    }

    //----< function dispatched by child thread to main thread >-------

    private void addStatus(string status)
    {
      StatusBarText.Items.Add(status);
    }

    //----< function dispatched by child thread to main thread >-------

    private void clearDirs()
    {
      DirList2.Items.Clear();
    }
    //----< function dispatched by child thread to main thread >-------

    private void addDir(string dir)
    {
      DirList2.Items.Add(dir);
    }
    //----< function dispatched by child thread to main thread >-------

    private void insertParent()
    {
      DirList2.Items.Insert(0, "..");
    }
    //----< function dispatched by child thread to main thread >-------

    private void clearFiles()
    {
      FileList2.Items.Clear();
    }
    //----< function dispatched by child thread to main thread >-------

    private void addFile(string file)
    {
      FileList2.Items.Add(file);
    }
    //----< add client processing for message with key >---------------

    private void addClientProc(string key, Action<CsMessage> clientProc)
    {
      dispatcher_[key] = clientProc;
    }
    //----< load getDirs processing into dispatcher dictionary >-------

    private void DispatcherLoadGetDirs()
    {
      Action<CsMessage> getDirs = (CsMessage rcvMsg) =>
      {
        Action clrDirs = () =>
        {
          clearDirs();
        };
        Dispatcher.Invoke(clrDirs, new Object[] { });
        var enumer = rcvMsg.attributes.GetEnumerator();
        while (enumer.MoveNext())
        {
          string key = enumer.Current.Key;
          if (key.Contains("dir"))
          {
            Action<string> doDir = (string dir) =>
            {
              addDir(dir);
            };
            Dispatcher.Invoke(doDir, new Object[] { enumer.Current.Value });
          }
        }
        Action insertUp = () =>
        {
          insertParent();
        };
        Dispatcher.Invoke(insertUp, new Object[] { });
      };
      addClientProc("getDirs", getDirs);
    }
    //----< load getFiles processing into dispatcher dictionary >------

    private void DispatcherLoadGetFiles()
    {
      Action<CsMessage> getFiles = (CsMessage rcvMsg) =>
      {
        Action clrFiles = () =>
        {
          clearFiles();
        };
        Dispatcher.Invoke(clrFiles, new Object[] { });
        var enumer = rcvMsg.attributes.GetEnumerator();
        while (enumer.MoveNext())
        {
          string key = enumer.Current.Key;
          if (key.Contains("file"))
          {
            Action<string> doFile = (string file) =>
            {
              addFile(file);
            };
            Dispatcher.Invoke(doFile, new Object[] { enumer.Current.Value });
          }
        }
      };
      addClientProc("getFiles", getFiles);
    }

    //----< load reCheckIn processing into dispatcher dictionary >------

    private void DispatcherLoadreCheckIn()
    {
      Action<CsMessage> reCheckIn = (CsMessage rcvMsg) =>
      {
        Action<string> doStatus = (string status) =>
        {
          addStatus(status);
        };
        Dispatcher.Invoke(doStatus, new Object[] { "Check-In success!" });
      };
      addClientProc("reCheckIn", reCheckIn);
    }

    //----< load reCheckOut processing into dispatcher dictionary >------

    private void DispatcherLoadreCheckOut()
    {
      Action<CsMessage> reCheckOut = (CsMessage rcvMsg) =>
      {
        Action<string> doStatus = (string status) =>
        {
          addStatus(status);
        };
        Dispatcher.Invoke(doStatus, new Object[] { "Check-Out success!" });
      };
      addClientProc("reCheckOut", reCheckOut);
    }

    //----< load reBrowse processing into dispatcher dictionary >------

    private void DispatcherLoadreBrowse()
    {
      Action<CsMessage> reBrowse = (CsMessage rcvMsg) =>
      {
        Action<string> doStatus = (string status) =>
        {
          addStatus(status);
        };
        Dispatcher.Invoke(doStatus, new Object[] { "Browse success!" });
      };
      addClientProc("reBrowse", reBrowse);
    }

    //----< load reMetadata processing into dispatcher dictionary >------

    private void DispatcherLoadreMetedata()
    {
      Action<CsMessage> reMetadata = (CsMessage rcvMsg) =>
      {
        Action<string> doStatus = (string status) =>
        {
          addStatus(status);
        };
        Dispatcher.Invoke(doStatus, new Object[] { "View-Metadata success!" });
      };
      addClientProc("reMetadata", reMetadata);
    }

    //----< load all dispatcher processing >---------------------------

    private void loadDispatcher()
    {
      DispatcherLoadGetDirs();
      DispatcherLoadGetFiles();
      DispatcherLoadreCheckIn();
      DispatcherLoadreCheckOut();
      DispatcherLoadreBrowse();
      DispatcherLoadreMetedata();

    }
    //----< start Comm, fill window display with dirs and files >------

    private void Window_Loaded(object sender, RoutedEventArgs e)
    {
      endPoint_ = new CsEndPoint();
      endPoint_.machineAddress = "localhost";
      endPoint_.port = 8084;
      translater = new Translater();
      translater.listen(endPoint_);

      // start processing messages
      processMessages();

      // load dispatcher
      loadDispatcher();


      foreach (string t in System.IO.Directory.GetFiles("../../../ClientStr2"))
      {
        FileList1.Items.Add(t.Remove(0, 20));
      }

      DirList1.Items.Add("..");
      foreach (string k in System.IO.Directory.GetDirectories("../../../ClientStr2"))
      {
        DirList1.Items.Add(k.Remove(0, 20));
      }

      Console.WriteLine(System.Environment.NewLine);
      Console.WriteLine("  Testing Remote Repository Prototypes");
      process();
    }
    //----< strip off name of first part of path >---------------------

    private string removeFirstDir(string path)
    {
      string modifiedPath = path;
      int pos = path.IndexOf("/");
      modifiedPath = path.Substring(pos + 1, path.Length - pos - 1);
      return modifiedPath;
    }

    //----< respond to mouse double-click on server file name >----------------
    private void FileList2_MouseDoubleClick(object sender, MouseButtonEventArgs e)
    {
      fileName3.Text = (string)FileList2.SelectedItem;
      fileName2.Text = (string)FileList2.SelectedItem;
      metedataText.Text = (string)FileList2.SelectedItem;
    }

    //----< respond to mouse double-click on server dir name >----------------

    private void DirList2_MouseDoubleClick(object sender, MouseButtonEventArgs e)
    {
      // build path for selected dir
      string selectedDir = (string)DirList2.SelectedItem;
      string path;
      if (selectedDir == "..")
      {
        if (pathStack_.Count > 1)  // don't pop off "Storage"
          pathStack_.Pop();
        else
          return;
      }
      else
      {
        path = pathStack_.Peek() + "/" + selectedDir;
        pathStack_.Push(path);
      }
      // display path in Dir TextBlcok
      PathTextBlock2.Text = removeFirstDir(pathStack_.Peek());

      // build message to get dirs and post it
      CsEndPoint serverEndPoint = new CsEndPoint();
      serverEndPoint.machineAddress = "localhost";
      serverEndPoint.port = 8080;
      CsMessage msg = new CsMessage();
      msg.add("to", CsEndPoint.toString(serverEndPoint));
      msg.add("from", CsEndPoint.toString(endPoint_));
      msg.add("command", "getDirs");
      msg.add("path", pathStack_.Peek());
      translater.postMessage(msg);

      // build message to get files and post it
      msg.remove("command");
      msg.add("command", "getFiles");
      translater.postMessage(msg);
    }

    //----< respond to mouse click on Connect >----------------

    private void Connect_Click(object sender, RoutedEventArgs e)
    {
      CsEndPoint serverEndPoint = new CsEndPoint();
      serverEndPoint.machineAddress = "localhost";
      serverEndPoint.port = 8080;

      PathTextBlock2.Text = "Storage";
      pathStack_.Push("../Storage");
      CsMessage msg = new CsMessage();
      msg.add("to", CsEndPoint.toString(serverEndPoint));
      msg.add("from", CsEndPoint.toString(endPoint_));
      msg.add("command", "getDirs");
      msg.add("path", pathStack_.Peek());
      Console.WriteLine("\n  posting message in Translater");
      msg.show();
      translater.postMessage(msg);
      msg.remove("command");
      msg.add("command", "getFiles");
      Console.WriteLine("\n  posting message in Translater");
      msg.show();
      translater.postMessage(msg);

      isConnect = true;
    }

    //----< respond to mouse click on Check-In >----------------

    private void CheckIn_Click(object sender, RoutedEventArgs e)
    {
      Console.WriteLine("\n  Demonstrating Requirement #2 - Check-In");

      if (isConnect && (fileName1.Text != ""))
      {
        CsEndPoint serverEndPoint = new CsEndPoint();
        serverEndPoint.machineAddress = "localhost";
        serverEndPoint.port = 8080;

        CsMessage msg = new CsMessage();
        msg.add("to", CsEndPoint.toString(serverEndPoint));
        msg.add("from", CsEndPoint.toString(endPoint_));
        msg.add("command", "checkIn");
        msg.add("file", fileName1.Text);
        msg.add("srcPath", "../../../ClientStr2/");
        msg.add("rcvPath", "../Storage");
        Console.WriteLine("\n  posting message in Translater");
        msg.show();
        translater.postMessage(msg);
      }
    }

    //----< respond to mouse click on Check-Out >----------------

    private void CheckOut_Click(object sender, RoutedEventArgs e)
    {
      Console.WriteLine("\n  Demonstrating Requirement #3 - Check-Out");

      CsEndPoint serverEndPoint = new CsEndPoint();
      serverEndPoint.machineAddress = "localhost";
      serverEndPoint.port = 8080;

      CsMessage msg = new CsMessage();
      msg.add("to", CsEndPoint.toString(serverEndPoint));
      msg.add("from", CsEndPoint.toString(endPoint_));
      msg.add("command", "checkOut2");
      msg.add("fname", "Message.h");

      Console.WriteLine("\n  posting message in Translater");
      msg.show();
      translater.postMessage(msg);
    }

    //----< respond to mouse click on client refresh >----------------

    private void Refresh1_Click(object sender, RoutedEventArgs e)
    {
      DirList1.Items.Clear();

      DirList1.Items.Add("..");
      foreach (string k in System.IO.Directory.GetDirectories("../../../ClientStr2"))
      {
        DirList1.Items.Add(k.Remove(0, 20));
      }
      FileList1.Items.Clear();

      foreach (string t in System.IO.Directory.GetFiles("../../../ClientStr2"))
      {
        FileList1.Items.Add(t.Remove(0, 20));
      }
    }

    //----< respond to mouse click on server refresh >----------------

    private void Refresh2_Click(object sender, RoutedEventArgs e)
    {
      CsEndPoint serverEndPoint = new CsEndPoint();
      serverEndPoint.machineAddress = "localhost";
      serverEndPoint.port = 8080;

      PathTextBlock2.Text = "Storage";
      pathStack_.Push("../Storage");
      CsMessage msg = new CsMessage();
      msg.add("to", CsEndPoint.toString(serverEndPoint));
      msg.add("from", CsEndPoint.toString(endPoint_));
      msg.add("command", "getDirs");
      msg.add("path", pathStack_.Peek());
      translater.postMessage(msg);
      msg.remove("command");
      msg.add("command", "getFiles");
      translater.postMessage(msg);
    }

    //----< respond to mouse click on view file >----------------

    private void Browse_Click(object sender, RoutedEventArgs e)
    {
      Console.WriteLine("\n  Demonstrating Requirement #4 - Browsing & View-File");

      string selectedFileName = (string)FileList2.SelectedItem;
      CsEndPoint serverEndPoint = new CsEndPoint();
      serverEndPoint.machineAddress = "localhost";
      serverEndPoint.port = 8080;

      CsMessage msg = new CsMessage();
      msg.add("to", CsEndPoint.toString(serverEndPoint));
      msg.add("from", CsEndPoint.toString(endPoint_));
      msg.add("command", "browse2");
      msg.add("fname", selectedFileName);

      Console.WriteLine("\n  posting message in Translater");
      msg.show();
      translater.postMessage(msg);

      Thread.Sleep(500);

      string contents = File.ReadAllText("../../../temp/" + selectedFileName);
      Action showfile = () =>
      {
        showFile(selectedFileName, contents);
      };
      Dispatcher.Invoke(showfile, new Object[] { });
    }

    //----< respond to mouse click on view metadata >----------------

    private void Metadata_Click(object sender, RoutedEventArgs e)
    {
      Console.WriteLine("\n  Demonstrating Requirement #5 - Viewing Metadata");

      CsEndPoint serverEndPoint = new CsEndPoint();
      serverEndPoint.machineAddress = "localhost";
      serverEndPoint.port = 8080;

      CsMessage msg = new CsMessage();
      msg.add("to", CsEndPoint.toString(serverEndPoint));
      msg.add("from", CsEndPoint.toString(endPoint_));
      msg.add("command", "metadata");

      Console.WriteLine("\n  posting message in Translater");
      msg.show();
      translater.postMessage(msg);
    }


    //----< 1st test for connect button>---------------------------------
    void test1()
    {
      //connect.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, connect)); 

      Action showfile = () =>
      {
        RoutedEventArgs t = new RoutedEventArgs();
        Connect_Click(null, t);
      };
      Dispatcher.Invoke(showfile, new Object[] { });
    }

    //----< 2nd test for check-in button >---------------------------------
    void test2()
    {
      Thread.Sleep(500);
      Action showfile = () =>
      {
        Checkin.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, Checkin));
      };
      Dispatcher.Invoke(showfile, new Object[] { });
    }

    //----< 3rd test for check-out button >---------------------------------
    void test3()
    {
      Thread.Sleep(500);
      Action showfile = () =>
      {
        checkOut.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, checkOut));
      };
      Dispatcher.Invoke(showfile, new Object[] { });
    }

    //----< 4th test for browse & view-file button >---------------------------------
    void test4()
    {
      Thread.Sleep(500);
      Action showfile = () =>
      {
        FileList2.SelectedIndex = 5;
        viewFile.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, viewFile));
      };
      Dispatcher.Invoke(showfile, new Object[] { });

    }

    //----< 5th test for view-metadata button >---------------------------------
    void test5()
    {
      Thread.Sleep(500);
      Action showfile = () =>
      {
        viewMetadata.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, viewMetadata));
      };
      Dispatcher.Invoke(showfile, new Object[] { });
    }
  }

}


