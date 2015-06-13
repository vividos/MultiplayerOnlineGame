using System;
using System.Collections.Generic;
using System.IO;
using System.Text;

namespace BuildData
{
   class Program
   {
      static int Main(string[] args)
      {
         Console.WriteLine("MultiplayerOnlineGame - BuildData Tool");
         Console.WriteLine();

         if (args.Length != 3)
         {
            Console.WriteLine("Error: Must provide 3 arguments:");
            Console.WriteLine("BuildData.exe ConfigFile.cfg SourcePath\\ OutputPath\\");
            return 1;
         }

         var p = new Program();
         p.ConfigFile = args[0];
         p.SourcePath = args[1];
         p.OutputPath = args[2];
         p.AppPath = Path.GetDirectoryName(p.GetType().Assembly.Location);

         if (!Directory.Exists(p.OutputPath))
            Directory.CreateDirectory(p.OutputPath);

         Console.WriteLine("ConfigFile: {0}", p.ConfigFile);
         Console.WriteLine("SourcePath: {0}", p.SourcePath);
         Console.WriteLine("OutputPath: {0}", p.OutputPath);
         Console.WriteLine();

         try
         {
            p.Run();
            return 0;
         }
         catch (Exception ex)
         {
            Console.WriteLine("Error: " + ex.Message);
            Console.WriteLine("Exiting.");
            return 1;
         }
      }

      public string ConfigFile { get; set; }
      public string OutputPath { get; set; }
      public string SourcePath { get; set; }
      public string AppPath { get; set; }

      private void Run()
      {
         var sr = new StreamReader(this.ConfigFile, Encoding.UTF8);
         while (!sr.EndOfStream)
         {
            string line = sr.ReadLine();

            // remove comments
            int pos = line.IndexOf("//");
            if (pos != -1)
               line = line.Substring(0, pos);

            pos = line.IndexOf("#");
            if (pos != -1)
               line = line.Substring(0, pos);

            line = line.Trim();

            if (line.Length == 0)
               continue;

            string command = line.Substring(0, line.IndexOf(' '));
            line = line.Substring(command.Length + 1);

            var parameter = SplitParameter(line);

            try
            {
               ExecCommand(command, parameter.ToArray());
            }
            catch (Exception ex)
            {
               Console.WriteLine("Error executing command [{0}]: {1}", command, ex.Message);
            }
            Console.WriteLine();
         }
         Console.WriteLine("Done.");
      }

      private static List<string> SplitParameter(string line)
      {
         line = line.Trim() + " ";

         // split parameter
         var parameter = new List<string>();
         while (line.Length > 0)
         {
            line = line.TrimStart();

            char chStopper = ' ';
            for (int i = 0; i < line.Length; i++)
            {
               char ch = line[i];
               if (ch == chStopper)
               {
                  // new parameter
                  parameter.Add(line.Substring(0, i));
                  line = line.Substring(i + 1);
                  break;
               }

               if (ch == '"')
               {
                  // opening or closing quotes encountered
                  if (ch == chStopper)
                  {
                     // closing quotes
                     chStopper = ' ';
                  }
                  else
                     chStopper = ch;

                  line = line.Remove(i, 1);
                  i--;
               }
            }
         }

         return parameter;
      }

      private void ExecCommand(string command, string[] parameter)
      {
         switch (command)
         {
            case "copy":
               CopyFiles(parameter);
               break;

            case "wav-to-ogg":
               ConvertWaveToOggVorbis(parameter, false);
               break;

            case "wav-to-ogg-mono":
               ConvertWaveToOggVorbis(parameter, true);
               break;

            case "mp3-to-ogg":
               ConvertMp3ToOggVorbis(parameter);
               break;

            default:
               throw new ArgumentException("invalid command: " + command);
         }
      }

      private void CopyFiles(string[] parameter)
      {
         if (parameter.Length != 2)
            throw new ArgumentException("copy needs 2 arguments: source-wildcard-list relative-target-path");

         var fileList = GetFileList(parameter[0], this.SourcePath);

         Console.WriteLine("Copy [$(SourcePath)\\{0}, {1} files] => [$(OutputPath)\\{2}]",
             parameter[0], fileList.Length, parameter[1]);

         foreach (string relFilename in fileList)
         {
            string sourceFilename = Path.Combine(this.SourcePath, relFilename);
            string targetFilename = Path.Combine(this.OutputPath, relFilename);

            // check if folder already exists
            string targetDir = Path.GetDirectoryName(targetFilename);

            if (!Directory.Exists(targetDir))
               Directory.CreateDirectory(targetDir);

            // check if update is needed
            bool bIsUpdateNeeded = IsNeededUpdate(sourceFilename, targetFilename);

            Console.WriteLine("  [$(SourcePath)\\{0}] => [$(OutputPath)\\{0}]{1}",
               relFilename,
               bIsUpdateNeeded ? "" : " -> already up-to date");

            if (bIsUpdateNeeded)
            {
               File.Copy(sourceFilename, targetFilename, true);
            }
         }
      }

      private void ConvertWaveToOggVorbis(string[] parameter, bool downmix)
      {
         if (parameter.Length != 2)
            throw new ArgumentException("copy needs 2 arguments: source-wildcard-list relative-target-path");

         var fileList = GetFileList(parameter[0], this.SourcePath);

         Console.WriteLine("Convert Wave to Ogg Vorbis{3} [$(SourcePath)\\{0}, {1} files] => [$(OutputPath)\\{2}]",
            parameter[0], fileList.Length, parameter[1], downmix ? ", downmix to mono" : "");

         foreach (string relFilename in fileList)
         {
            string sourceFilename = Path.Combine(this.SourcePath, relFilename);

            string relOutFilename = Path.ChangeExtension(relFilename, "ogg");
            string targetFilename = Path.Combine(this.OutputPath, relOutFilename);

            // check if folder already exists
            string targetDir = Path.GetDirectoryName(targetFilename);

            if (!Directory.Exists(targetDir))
               Directory.CreateDirectory(targetDir);

            // check if update is needed
            bool bIsUpdateNeeded = IsNeededUpdate(sourceFilename, targetFilename);

            Console.WriteLine("  [$(SourcePath)\\{0}] => [$(OutputPath)\\{1}]{2}",
               relFilename, relOutFilename,
               bIsUpdateNeeded ? "" : " -> already up-to date");

            if (bIsUpdateNeeded)
            {
               WaveToOggVorbis(sourceFilename, targetFilename, downmix);

               CopyLastWriteTime(sourceFilename, targetFilename);
            }
         }
      }

      private void WaveToOggVorbis(string sourceFilename, string targetFilename, bool downmix)
      {
         string arguments = string.Format("--quality 4 --output=\"{1}\" \"{0}\"",
             sourceFilename, targetFilename);
         if (downmix)
            arguments = "--downmix " + arguments;

         var p = new System.Diagnostics.Process();
         p.StartInfo.FileName = Path.Combine(this.AppPath, "oggenc2.exe");
         p.StartInfo.Arguments = arguments;
         p.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
         p.Start();
         p.WaitForExit();
         if (p.ExitCode != 0)
         {
             if (downmix && p.ExitCode == 1)
             {
                 Console.WriteLine("Wave file {0} is already mono, not downmixing...", sourceFilename);

                 // is already mono; don't downmix
                 WaveToOggVorbis(sourceFilename, targetFilename, false);
             }
             else
                 throw new Exception("oggenc2 returned error");
         }
      }

      private void ConvertMp3ToOggVorbis(string[] parameter)
      {
         if (parameter.Length != 2)
            throw new ArgumentException("copy needs 2 arguments: source-wildcard-list relative-target-path");

         var fileList = GetFileList(parameter[0], this.SourcePath);

         Console.WriteLine("Convert mp3 to Ogg Vorbis [$(SourcePath)\\{0}, {1} files] => [$(OutputPath)\\{2}]",
             parameter[0], fileList.Length, parameter[1]);

         foreach (string relFilename in fileList)
         {
            string sourceFilename = Path.Combine(this.SourcePath, relFilename);

            string relOutFilename = Path.ChangeExtension(relFilename, "ogg");
            string targetFilename = Path.Combine(this.OutputPath, relOutFilename);

            // check if folder already exists
            string targetDir = Path.GetDirectoryName(targetFilename);

            if (!Directory.Exists(targetDir))
               Directory.CreateDirectory(targetDir);

            // check if update is needed
            bool bIsUpdateNeeded = IsNeededUpdate(sourceFilename, targetFilename);

            Console.WriteLine("  [$(SourcePath)\\{0}] => [$(OutputPath)\\{1}]{2}",
               relFilename, relOutFilename,
               bIsUpdateNeeded ? "" : " -> already up-to date");

            if (bIsUpdateNeeded)
            {
               Mp3ToOggVorbis(sourceFilename, targetFilename);

               CopyLastWriteTime(sourceFilename, targetFilename);
            }
         }
      }

      private void Mp3ToOggVorbis(string sourceFilename, string targetFilename)
      {
         string targetWaveFilename = Path.ChangeExtension(targetFilename, "wav");
         try
         {
            Mp3ToWave(sourceFilename, targetWaveFilename);
            WaveToOggVorbis(targetWaveFilename, targetFilename, false);
         }
         finally
         {
            File.Delete(targetWaveFilename);
         }
      }

      private void Mp3ToWave(string sourceFilename, string targetFilename)
      {
         string arguments = string.Format("--output=wave:\"{1}\" \"{0}\"",
             sourceFilename, targetFilename);

         var p = new System.Diagnostics.Process();
         p.StartInfo.FileName = Path.Combine(this.AppPath, "madplay.exe");
         p.StartInfo.Arguments = arguments;
         p.StartInfo.WindowStyle = System.Diagnostics.ProcessWindowStyle.Hidden;
         p.Start();
         p.WaitForExit();
         if (p.ExitCode != 0)
            throw new Exception("madplay returned error");
      }

      private string[] GetFileList(string wildcardSpec, string relativeToPath)
      {
         string searchPath = relativeToPath;

         int pos = wildcardSpec.LastIndexOf(Path.DirectorySeparatorChar);
         if (pos != -1)
         {
            searchPath = Path.Combine(searchPath, wildcardSpec.Substring(0, pos + 1));
            wildcardSpec = wildcardSpec.Substring(pos + 1);
         }

         var fileList = Directory.GetFiles(searchPath, wildcardSpec, SearchOption.AllDirectories);

         // make relative again
         int posRelativeEnd = relativeToPath.Length;
         if (!relativeToPath.EndsWith(Path.DirectorySeparatorChar.ToString()))
            ++posRelativeEnd;

         for (int i = 0; i < fileList.Length; i++)
         {
            fileList[i] = fileList[i].Substring(posRelativeEnd);
         }

         return fileList;
      }


      /// <summary>
      /// Determines when an update for a file is needed
      /// </summary>
      /// <param name="sourceFilename"></param>
      /// <param name="targetFilename"></param>
      /// <returns></returns>
      static private bool IsNeededUpdate(string sourceFilename, string targetFilename)
      {
         if (!File.Exists(targetFilename))
            return true;

         if (!File.Exists(sourceFilename))
            return true;

         DateTime dtSource = new FileInfo(sourceFilename).LastWriteTime;
         DateTime dtTarget = new FileInfo(targetFilename).LastWriteTime;

         // source newer than target?
         return dtSource > dtTarget;
      }

      static private void CopyLastWriteTime(string sourceFilename, string targetFilename)
      {
         DateTime dtSource = new FileInfo(sourceFilename).LastWriteTime;
         new FileInfo(targetFilename).LastWriteTime = dtSource;
      }
   }
}
