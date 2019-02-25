///////////////////////////////////////////////////////////////////////
// CodePopUpWindow.xaml.cs - GUI for PopUp Window                    //
// ver 1.0                                                           //
// Datan Xu, CSE687 - Object Oriented Design, Spring 2018            //
///////////////////////////////////////////////////////////////////////
/*
 * Package Operations:
 * -------------------
 * This package provides a WPF-based GUI for PopUp Window.  It's 
 * responsibilities is to:
 * - open a new window displaying text of a Repository file will satisfy this requirement.
 * 
 *   
 * Required Files:
 * ---------------
 * CodePopUpWindow.xaml, CodePopUpWindow.xaml.cs
 * Translater.dll
 * 
 * Maintenance History:
 * --------------------
 * ver 1.0 : 5 Apr 2018
 * - first release
 */


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
using System.Windows.Shapes;

namespace WpfApp1
{
  /// <summary>
  /// CodePopUpWindow.xaml Initialize        
  /// </summary>
  public partial class CodePopUpWindow : Window
  {
    public CodePopUpWindow()
    {
      InitializeComponent();
    }
  }
}
