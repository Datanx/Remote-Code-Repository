# Remote Code Repository

## 1. The project mainly includes 4 parts:
   - GUI client
   - Server
   - MsgPassingComm
   - NoSqlDb
   
   ### Some supporting actors are included as well:
   - CommLibWrapper
   - DateTime
   - DbCore
   - FileSystem
   - PayLoad
   - Sockets
   - Translater
   - Utilities
   - XmlDocument
## 2. There are Tabs:
   - <b>Connect</b> to link to the remote repository.
   - <b>Check-In</b> to start a new file version. 
   - <b>Close</b> to close a file's check-in status.
   - <b>Check-Out</b> to retrieves package files, removing version information from their filenames. 
   - <b>View File</b> to view full file text
   - <b>Query</b> to display collections of filenames based on queries defining categories
   - <b>Client Dirs Refresh</b> to get local files and directories
   - <b>Server Dirs Refresh</b> to get remote code repository files and directories
   - <b>View Metedata</b> to view a specified file metadata
## 3. What it does do:
   - Shows local and remote directories and files.
   - Double-clicking on a directory navigates into that directory and shows its contents for both the local and remote locations.

     ### Check-In
   - To check in a file, you should type a file name in the text box and, type the owner name, description, one or more categories and namespace as check-in information.
   - Then, clicking on a file or more files in local navigator will select the file(s) as the dependencies of the check-in file.
   - In the end, clicking on Check-In button to check in the file.

     ### Check-Out
   - Double-Clicking on a file in remote navigator will select the file as the one to be checked out. Then clicking on Check-Out button to check out the file.
     
     ### View File
   - Double-Clicking on a file in remote navigator will select the file as the one to be viewed. Then clicking on View File button to view the file by a popup window.
    
     ### View Metedata
   - Double-Clicking on a file in remote navigator will select the file as the one to be viewed. Then clicking on View Metedata button to view the metedata of the file.

## 4. There is also a status bar to show responses from server.
