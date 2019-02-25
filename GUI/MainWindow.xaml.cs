///////////////////////////////////////////////////////////////////////
// MainWindow.xaml.cs - GUI for Project4                             //
// ver 2.0                                                           //
// Datan Xu, CSE687 - Object Oriented Design, Spring 2018            //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF-based GUI for Project4.  It's 
 * responsibilities are to:
 * - Provide a display of directory contents of a remote ServerPrototype.
 * - It provides a subdirectory list and a filelist for the selected directory.
 * - You can navigate into subdirectories by double-clicking on subdirectory
 *   or the parent directory, indicated by the name "..".
 *   
 * Required Files:
 * ---------------
 * Mainwindow.xaml, MainWindow.xaml.cs
 * Translater.dll
 * 
 * Maintenance History:
 * --------------------
 * ver 1.0 : 30 Mar 2018
 * - first release
 * - Several early prototypes were discussed in class. Those are all superceded
 *   by this package.
 *   
 * ver 2.0 : 27 April 2018 
 * - second release 
 * - More functionality has been implemented.
 */

// Translater has to be statically linked with CommLibWrapper
// - loader can't find Translater.dll dependent CommLibWrapper.dll
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

namespace WpfApp1
{
  public partial class MainWindow : Window
  {
    public MainWindow()
    {
      InitializeComponent();
    }

    private bool isConnect = false;
    private Stack<string> clientPathStack_ = new Stack<string>();

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
        Console.WriteLine("\n  Demonstrating Requirement #1 - Functionality to Connect Remote Repository");
        test1();          // test for connecting to the Remote Repository
        test2();          // test for the functionality to start a new file version 
        test3();          // test for version numbering for all files held in the Repository
        test4();          // test for retrieves package files, removing version information from their filenames.
        test5();          // test for displaying collections of filenames based on queries defining categories
        test6();          // test for viewing full file text
        test7();          // test for viewing view file metadata
        test8();
        test9();
        test10();
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

    //----< clear Browse listbox >-------

    private void clearBrowse()
    {
      Browse.Items.Clear();
    }

    //----< add items to Browse listbox>-------

    private void addBrowse(string key)
    {
      Browse.Items.Add(key);
    }

    //----< clear metedata listbox >-------

    private void clearMete()
    {
      Metedata.Items.Clear();
    }

    //----< add items to metedata listbox>-------

    private void addMete(string mete)
    {
      Metedata.Items.Add(mete);
    }

    //----< clear Dir listbox >-------

    private void clearDirs()
    {
      DirList2.Items.Clear();
    }

    //----< add items to Dir listbox >-------

    private void addDir(string dir)
    {
      DirList2.Items.Add(dir);
    }

    //----< function dispatched by child thread to main thread >-------

    private void insertParent()
    {
      DirList2.Items.Insert(0, "..");
    }

    //----< clear FileList2 listbox >-------

    private void clearFiles()
    {
      FileList2.Items.Clear();
    }
    //----< add items to FileList2 listbox>-------

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
        if(rcvMsg.value("flag") == "y")
        {
          Dispatcher.Invoke(doStatus, new Object[] { "Check-In success!" });
        }
        else
        {
          Dispatcher.Invoke(doStatus, new Object[] { "Check-In failed. The file dose not exist!"});
        }
      };
      addClientProc("reCheckIn", reCheckIn);
    }

    //----< load reCloseFile processing into dispatcher dictionary >------

    private void DispatcherLoadreCloseFile()
    {
      Action<CsMessage> reCloseFile = (CsMessage rcvMsg) =>
      {
        Action<string> doStatus = (string status) =>
        {
          addStatus(status);
        };
        if (rcvMsg.value("flag") == "y")
        {
          Dispatcher.Invoke(doStatus, new Object[] { rcvMsg.value("closedFile") + " has been closed!" });
        }
        else
        {
          Dispatcher.Invoke(doStatus, new Object[] { "Close failed. " + rcvMsg.value("closedFile") + " does not exist!" });
        }
      };
      addClientProc("reCloseFile", reCloseFile);
    }

    //----< load reCheckOut processing into dispatcher dictionary >------

    private void DispatcherLoadreCheckOut()
    {
      Action<CsMessage> reCheckOut = (CsMessage rcvMsg) =>
      {
        if (rcvMsg.value("parents") != "")
        {
          Action checkOut_Parents = () =>
          {
            string[] parents = rcvMsg.attributes["parents"].Split(' ');
            foreach (var fname in parents)
            {
              CsEndPoint serverEndPoint = new CsEndPoint();
              serverEndPoint.machineAddress = "localhost";
              serverEndPoint.port = 8080;

              CsMessage msg = new CsMessage();
              msg.add("to", CsEndPoint.toString(serverEndPoint));
              msg.add("from", CsEndPoint.toString(endPoint_));
              msg.add("command", "checkOutParents");
              msg.add("fname", fname);

              translater.postMessage(msg); 
            }
          };
          Dispatcher.Invoke(checkOut_Parents, new Object[] { });
        }
        Action<string> doStatus = (string status) =>
        {
          addStatus(status);
        };
        Dispatcher.Invoke(doStatus, new Object[] { "Check-Out success!" });
      };
      addClientProc("reCheckOut", reCheckOut);
    }

    //----< load reCheckOutParents processing into dispatcher dictionary >------

    private void DispatcherLoadreCheckOutParents()
    {
      Action<CsMessage> reCheckOutParents = (CsMessage rcvMsg) =>
      {
        Action<string> doStatus = (string status) =>
        {
          addStatus(status);
        };
        Dispatcher.Invoke(doStatus, new Object[] { "Parent file: " + rcvMsg.value("file") + " has been checked out!" });
      };
      addClientProc("reCheckOutParents", reCheckOutParents);
    }

    //----< load reQuery processing into dispatcher dictionary >------

    private void DispatcherLoadreQuery()
    {
      Action<CsMessage> reBrowse = (CsMessage rcvMsg) =>
      {
        Action clrBrowse = () =>
        {
          clearBrowse();
        };
        Dispatcher.Invoke(clrBrowse, new Object[] { });
        Action<string> doKeys = (string item) =>
        {
          addBrowse(item);
        };
        Dispatcher.Invoke(doKeys, new Object[] { rcvMsg.value("keys") });
          
        Action<string> doStatus = (string status) =>
        {
          addStatus(status);
        };
        Dispatcher.Invoke(doStatus, new Object[] { "Query category success!" });
      };
      addClientProc("reQuery", reBrowse);
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
        Action clrMetes = () =>
        {
          clearMete();
        };
        Dispatcher.Invoke(clrMetes, new Object[] { });

        var enumer = rcvMsg.attributes.GetEnumerator();
        while (enumer.MoveNext())
        {
          string key = enumer.Current.Key;
          Action<string> doMete = (string mete) =>
          {
            addMete(mete);
          };
          if (key.Contains("descrip"))
          {
            Dispatcher.Invoke(doMete, new Object[] { " Description: " + enumer.Current.Value });
          }
          else if (key.Contains("namespace"))
          {
            Dispatcher.Invoke(doMete, new Object[] { " Namespace: " + enumer.Current.Value });
          }
          else if (key.Contains("cate"))
          {
            Dispatcher.Invoke(doMete, new Object[] { " Category: " + enumer.Current.Value });
          }
          else if (key.Contains("time"))
          {
            Dispatcher.Invoke(doMete, new Object[] { " Time: " + enumer.Current.Value });
          }
          else if (key.Contains("depend"))
          {
            Dispatcher.Invoke(doMete, new Object[] { " Dependencies: " + enumer.Current.Value });
          }
          else
            continue;
        }

        Action<string> doStatus = (string status) =>
        {
          addStatus(status);
        };
        Dispatcher.Invoke(doStatus, new Object[] { "View-Metadata success!" });
      };
      addClientProc("reMetedata", reMetadata);
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
      DispatcherLoadreCloseFile();
      DispatcherLoadreCheckOutParents();
      DispatcherLoadreQuery();

    }

    //----< start Comm, fill window display with dirs and files >------

    private void Window_Loaded(object sender, RoutedEventArgs e)
    {
      endPoint_ = new CsEndPoint();
      endPoint_.machineAddress = "localhost";
      endPoint_.port = 8082;
      translater = new Translater();
      translater.listen(endPoint_);

      // start processing messages
      processMessages();

      // load dispatcher
      loadDispatcher();

      clientPathStack_.Push("../../../../ClientStr");

      foreach (string t in System.IO.Directory.GetFiles("../../../../ClientStr"))
      {
        FileList1.Items.Add(t.Remove(0,22));
      }

      DirList1.Items.Add("..");
      foreach (string k in System.IO.Directory.GetDirectories("../../../../ClientStr"))
      {
        DirList1.Items.Add(k.Remove(0,22));
      }
        
      Console.WriteLine(System.Environment.NewLine);
      Console.WriteLine("  Testing Remote Repository Prototypes");

      process();        // test process
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
      if(selectedDir == "..")
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

    //----< respond to mouse double-click on client dir name >----------------

    private void DirList1_MouseDoubleClick(object sender, MouseButtonEventArgs e)
    {
      // build path for selected dir
      string selectedDir = (string)DirList1.SelectedItem;
      string path;
      if (selectedDir == "..")
      {
        if (clientPathStack_.Count > 1)  // don't pop off "Storage"
          clientPathStack_.Pop();
        else
          return;
      }
      else
      {
        path = clientPathStack_.Peek() + "/" + selectedDir;
        clientPathStack_.Push(path);
      }
      // display path in Dir TextBlcok
      PathTextBlock1.Text = removeFirstDir(clientPathStack_.Peek());

      // build message to get dirs and post it
      DirList1.Items.Clear();

      DirList1.Items.Add("..");
      foreach (string k in System.IO.Directory.GetDirectories(clientPathStack_.Peek()))
      {
        DirList1.Items.Add(k.Remove(0, 22));
      }
      FileList1.Items.Clear();

      foreach (string t in System.IO.Directory.GetFiles(clientPathStack_.Peek()))
      {
        FileList1.Items.Add(t.Remove(0, 22));
      }
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
        msg.add("owner", author.Text);
        msg.add("descript", descp.Text);
        msg.add("cate", cat.Text);
      
        dependent.Clear();
        if (FileList1.SelectedItems.Count < 1)
        {
          dependent.Text = "";
        }
        else
        {
          foreach (object obj in FileList1.SelectedItems)
          {
            dependent.Text += obj.ToString() + " ";
          }
        }

        msg.add("dependencies", dependent.Text);
        FileList1.SelectedItems.Clear();
        msg.add("namespace", nameSpace.Text);
        msg.add("srcPath", "../../../../ClientStr/");
        msg.add("rcvPath", "../TempForServer/");
        Console.WriteLine("\n  posting message in Translater");
        msg.show();
        translater.postMessage(msg);
      }
    }

    //----< respond to mouse click on Check-Out >----------------

    private void CheckOut_Click(object sender, RoutedEventArgs e)
    {
      if (isConnect && (fileName2.Text != ""))
      {
        CsEndPoint serverEndPoint = new CsEndPoint();
        serverEndPoint.machineAddress = "localhost";
        serverEndPoint.port = 8080;

        CsMessage msg = new CsMessage();
        msg.add("to", CsEndPoint.toString(serverEndPoint));
        msg.add("from", CsEndPoint.toString(endPoint_));
        msg.add("command", "checkOut");
        msg.add("fname", fileName2.Text);

        Console.WriteLine("\n  posting message in Translater");
        msg.show();
        translater.postMessage(msg);
      }
    }

    //----< respond to mouse click on client refresh >----------------

    private void Refresh1_Click(object sender, RoutedEventArgs e)
    {
      DirList1.Items.Clear();

      DirList1.Items.Add("..");
      foreach (string k in System.IO.Directory.GetDirectories("../../../../ClientStr"))
      {
        DirList1.Items.Add(k.Remove(0, 22));
      }
      FileList1.Items.Clear();

      foreach (string t in System.IO.Directory.GetFiles("../../../../ClientStr"))
      {
        FileList1.Items.Add(t.Remove(0, 22));
      }
    }

    //----< respond to mouse click on server refresh >----------------

    private void Refresh2_Click(object sender, RoutedEventArgs e)
    {
      if (isConnect)
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
    }

    //----< respond to mouse click on Query >----------------

    private void Query_Click(object sender, RoutedEventArgs e)
    {
      if (isConnect && (Cate.Text != ""))
      {
        string selectedFileName = (string)FileList2.SelectedItem;
        CsEndPoint serverEndPoint = new CsEndPoint();
        serverEndPoint.machineAddress = "localhost";
        serverEndPoint.port = 8080;

        CsMessage msg = new CsMessage();
        msg.add("to", CsEndPoint.toString(serverEndPoint));
        msg.add("from", CsEndPoint.toString(endPoint_));
        msg.add("command", "query");
        msg.add("category", Cate.Text);

        Console.WriteLine("\n  posting message in Translater");
        msg.show();
        translater.postMessage(msg);
      }
    }

    //----< respond to mouse click on view file >----------------

    private void Browse_Click(object sender, RoutedEventArgs e)
    {
      if (isConnect && (fileName3.Text != ""))
      {
        string selectedFileName = (string)FileList2.SelectedItem;
        CsEndPoint serverEndPoint = new CsEndPoint();
        serverEndPoint.machineAddress = "localhost";
        serverEndPoint.port = 8080;

        CsMessage msg = new CsMessage();
        msg.add("to", CsEndPoint.toString(serverEndPoint));
        msg.add("from", CsEndPoint.toString(endPoint_));
        msg.add("command", "browse");
        msg.add("fname", selectedFileName);

        Console.WriteLine("\n  posting message in Translater");
        msg.show();
        translater.postMessage(msg);

        Thread.Sleep(500);

        string contents = File.ReadAllText("../../../../TempForClient/" + selectedFileName);
        Action showfile = () =>
        {
          showFile(selectedFileName, contents);
        };
        Dispatcher.Invoke(showfile, new Object[] { });
      }
    }

    //----< respond to mouse click on Close >----------------

    private void Close_Click(object sender, RoutedEventArgs e)
    {
      if (isConnect && (fileName1.Text != ""))
      {
        CsEndPoint serverEndPoint = new CsEndPoint();
        serverEndPoint.machineAddress = "localhost";
        serverEndPoint.port = 8080;

        CsMessage msg = new CsMessage();
        msg.add("to", CsEndPoint.toString(serverEndPoint));
        msg.add("from", CsEndPoint.toString(endPoint_));
        msg.add("command", "closeFile");
        msg.add("closeFname", fileName1.Text);

        Console.WriteLine("\n  posting message in Translater");
        msg.show();
        translater.postMessage(msg);
      }
    }

    //----< respond to mouse click on view metadata >----------------

    private void Metadata_Click(object sender, RoutedEventArgs e)
    {
      if (isConnect && (metedataText.Text != ""))
      {
        Console.WriteLine("\n  Demonstrating Requirement #5 - Viewing Metadata");

        CsEndPoint serverEndPoint = new CsEndPoint();
        serverEndPoint.machineAddress = "localhost";
        serverEndPoint.port = 8080;

        CsMessage msg = new CsMessage();
        msg.add("to", CsEndPoint.toString(serverEndPoint));
        msg.add("from", CsEndPoint.toString(endPoint_));
        msg.add("command", "metedata");
        msg.add("fName", metedataText.Text);

        Console.WriteLine("\n  posting message in Translater");
        msg.show();
        translater.postMessage(msg);
      }
    }


    //----< 1st test for functionality to connect Remote Repository>---------------------------------
    void test1()
    { 
      Action showfile = () =>
      {
        RoutedEventArgs t = new RoutedEventArgs();
        Connect_Click(null, t);
      };
      Dispatcher.Invoke(showfile, new Object[] { });
    }

    //----< 2nd test for functionality to check-in >---------------------------------
    void test2()
     {
      Thread.Sleep(500);
      Action checkIn = () =>
      {
        Console.WriteLine("\n  Demonstrating Requirement #2a - Functionality to check-in");
        Console.WriteLine("\n  Check in \"Sockets.h\" for the First Time");
        Console.WriteLine("----------------------------------------------");
        fileName1.Text = "Sockets.h";
        author.Text = "Datan";
        descp.Text = "Four classes that wrap the Winsock API";
        cat.Text = "cate1 cate2";
        dependent.Text = "";
        nameSpace.Text = "Sockets";
        Checkin.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, Checkin));
        
        Thread.Sleep(500);
        Console.WriteLine("\n  Check in \"Sockets.cpp\" for the First Time");
        Console.WriteLine("----------------------------------------------");
        fileName1.Text = "Sockets.cpp";
        author.Text = "Datan";
        descp.Text = "Server's client handler class";
        cat.Text = "cate1 cate2";
      
        FileList1.SelectedItems.Add(FileList1.Items[5]);
        nameSpace.Text = "Sockets";
        Checkin.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, Checkin));
        Refresh2.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, Refresh2));
        Console.WriteLine("\n  Meet the requirement for transfering source code text files from a local Client to Remote Code Repository.");
        Console.WriteLine("---------------------------------------------------------------------------------------------------------------");
      };
      Dispatcher.Invoke(checkIn, new Object[] { });
    }

    //----< 3rd test for functionality to manage version numbering for all files >---------------------------------
    void test3()
    {
      Thread.Sleep(500);
      Action checkIn = () =>
      {
        Console.WriteLine("\n  Demonstrating Requirement #2b - Functionality to Manage Version Numbering");
        Console.WriteLine("\n  Close the check-in for \"Sockets.h.1\"");
        Console.WriteLine("-------------------------------------------");
        fileName1.Text = "Sockets.h";
        Close.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, Close));
        Console.WriteLine("\n  Check in \"Sockets.h\" for the Second Time");
        Console.WriteLine("-----------------------------------------------");
        author.Text = "Datan";
        descp.Text = "Provides four classes wrap the Winsock API";
        cat.Text = "cate2 cate3";
        dependent.Text = "";
        nameSpace.Text = "Sockets";
        Checkin.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, Checkin));
        Refresh2.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, Refresh2));

        Thread.Sleep(500);
        Console.WriteLine("\n  Check in \"Sockets.h\" for the Third Time, note that \"Sockets.h.2\" is Open");
        Console.WriteLine("----------------------------------------------------------------------------------");
        Checkin.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, Checkin));
        Refresh2.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, Refresh2));
        Console.WriteLine("\n  \"Sockets.h.2\" with open check-in can be modified without altering its version number");
        Console.WriteLine("----------------------------------------------------------------------------------------------");
      };
      Dispatcher.Invoke(checkIn, new Object[] { });
    }

    //----< 4th test for functionality to check out >---------------------------------
    void test4()
    {
      Console.WriteLine("\n  Demonstrating Requirement #2c - Functionality to check-out");
      Thread.Sleep(500);
      Action showfile = () =>
      {
        Console.WriteLine("\n  Check out \"Sockets.cpp.1\" with dependency \"Sockets.h.1\" to \"Checked_Out_Files\" directory");
        Console.WriteLine("-----------------------------------------------------------------------------------------------------");
        FileList1.SelectedItems.Add(FileList1.Items[0]);
        fileName2.Text = "Sockets.cpp.1";
        checkOut.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, checkOut));
        Console.WriteLine("\n  Meet the requirement for transfering source code text files from Remote Code Repository to a local Client.");
        Console.WriteLine("----------------------------------------------------------------------------------------------------------------");
      };
      Dispatcher.Invoke(showfile, new object[] { });
    }

    //----< 5th test for functionality to query >---------------------------------
    void test5()
    {
      Thread.Sleep(1000);
      Action showfile = () =>
      {
        Console.WriteLine("\n  Demonstrating Requirement #2d - Functionality to Query based on categories");
        Console.WriteLine("\n  Displaying collections of filenames based on queries defining category \"cate2\"");
        Console.WriteLine("--------------------------------------------------------------------------------------");
        Cate.Text = "cate2";
        Query.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, Query));
        Console.WriteLine("\n  \"Sockets.h\" and \"Sockets.cpp\" should be displayed on the Browse GUI");
        Console.WriteLine("-----------------------------------------------------------------------------");
      };
      Dispatcher.Invoke(showfile, new Object[] { });
    }

    //----< 6th test for functionality to browse(view) a file text >---------------------------------
    void test6()
    {
      Thread.Sleep(500);
      Action showfile = () =>
      {
        Console.WriteLine("\n  Demonstrating Requirement #3a - Functionality to Browse(view) a file text");
        Console.WriteLine("\n  Displaying \"Sockets.h.1\" in popup window");
        Console.WriteLine("-----------------------------------------------");
        fileName3.Text = "Sockets.h.1";
        FileList2.SelectedIndex = 1;
        viewFile.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, viewFile));
      };
      Dispatcher.Invoke(showfile, new Object[] { });
    }

    //----< 7th test for functionality to view file metadata >---------------------------------
    void test7()
    {
      Thread.Sleep(500);
      Action showfile = () =>
      {
        Console.WriteLine("\n  Demonstrating Requirement #3b - Functionality to view file metadata");
        Console.WriteLine("\n  Displaying metedata for \"Sockets.cpp.1\" on the View Metadata GUI");
        Console.WriteLine("-----------------------------------------------------------------------");
        metedataText.Text = "Sockets.cpp.1";
        FileList2.SelectedIndex = 0;
        viewMetedata.RaiseEvent(new RoutedEventArgs(Button.ClickEvent, viewMetedata));
      };
      Dispatcher.Invoke(showfile, new Object[] { });
    }

    //----< 8th test for functionality to display message-passing communication system >---------------------------------
    void test8()
    {
      Thread.Sleep(500);
      Action showfile = () =>
      {
        Console.WriteLine("\n  Demonstrating Requirement #4 - Provide a message-passing communication system");
        Console.WriteLine("\n  MsgPassingComm package defines Sender and Receiver classes.");
        Console.WriteLine("\n  Sender uses a SocketConnecter and supports connecting to multiple sequential endpoints and posting messages.");
        Console.WriteLine("\n  Receiver uses a SocketListener which returns a Socket on connection.");
        Console.WriteLine("\n  It also defines a Comm class. Comm simply composes a Sender and a Receiver, exposing methods: postMessage(Message) and getMessage()");
        Console.WriteLine("------------------------------------------------------------------------------------------------------------------------------------------");
      };
      Dispatcher.Invoke(showfile, new Object[] { });
    }

    //----< 9th test for functionality to show HTTP style messages using asynchronous one-way messaging. >---------------------------------
    void test9()
    {
      Thread.Sleep(500);
      Action showfile = () =>
      {
        Console.WriteLine("\n  Demonstrating Requirement #5 - HTTP style messages using asynchronous one-way messaging");
        Console.WriteLine("\n  Message package defines an EndPoint struct and a Message class.");
        Console.WriteLine("\n  Endpoints define a message source or destination with an address and port number.");
        Console.WriteLine("\n  Messages have an HTTP style structure with a set of attribute lines containing name: value pairs.");
        Console.WriteLine("--------------------------------------------------------------------------------------------------------");
      };
      Dispatcher.Invoke(showfile, new Object[] { });
    }

    //----< 10th test for transmission of files >---------------------------------
    void test10()
    {
      Thread.Sleep(500);
      Action showfile = () =>
      {
        Console.WriteLine("\n  Demonstrating Requirement #6 -  Transmission of Files.");
        Console.WriteLine("\n  Check-In, Check-Out and Browse are based on sending and receiving blocks of bytes to support file transfer.");
        Console.WriteLine("----------------------------------------------------------------------------------------------------------------");
      };
      Dispatcher.Invoke(showfile, new Object[] { });
    }
  }
}
