using System;
using System.IO;

namespace EncodeFile
{
    class Program
    {
        static int Main(string[] args)
        {
            Console.WriteLine("MultiplayerOnlineGame - EncodeFile Tool");
            Console.WriteLine();

            if (args.Length == 1 && args[0].Contains("generate"))
            {
                GenerateKey();
                return 0;
            }

            var p = new Program();

            try
            {
                p.Encode(args);

                return 0;
            }
            catch (Exception ex)
            {
                Console.WriteLine("Error: " + ex.Message);
                Console.WriteLine("Exiting.");

                return 1;
            }
        }

        private static void GenerateKey()
        {
            var r = new Random();
            byte[] key = new byte[19];
            r.NextBytes(key);

            foreach (var item in key)
            {
                Console.Write(item);
                Console.Write(", ");
            }
            Console.WriteLine();
        }

        private byte[] encodeKey = { 240, 59, 117, 142, 148, 192, 244, 244, 25, 47, 54, 229, 223, 162, 66, 201, 28, 251, 16 };

        private void Encode(string[] fileList)
        {
            foreach (string filenameWithWildcards in fileList)
            {
                var filenamesList = Directory.GetFiles(
                    Path.GetDirectoryName(filenameWithWildcards),
                    Path.GetFileName(filenameWithWildcards), SearchOption.AllDirectories);

                foreach (var filename in filenamesList)
                {
                    string targetFilename = filename + "x";

                    Console.WriteLine("Encoding file {0}", targetFilename);

                    EncodeFile(filename, targetFilename);
                }
            }
        }

        private void EncodeFile(string filename, string targetFilename)
        {
            using (var inputStream = new FileStream(filename, FileMode.Open))
            using (var outputStream = new FileStream(targetFilename, FileMode.Create))
                Encode(inputStream, outputStream);
        }

        private void Encode(FileStream inputStream, FileStream outputStream)
        {
            long fileLength = inputStream.Length;
            byte[] buffer = new byte[1024];
            int currentPos = 0;
            while (inputStream.Position < fileLength)
            {
                int readLength = inputStream.Read(buffer, 0, buffer.Length);
                if (readLength == 0)
                    return;

                EncodeBuffer(ref buffer, readLength, ref currentPos);

                outputStream.Write(buffer, 0, readLength);
            }
        }

        private void EncodeBuffer(ref byte[] buffer, int length, ref int currentPos)
        {
            for (int i = 0; i < length; i++)
            {
                buffer[i] ^= encodeKey[currentPos++];

                if (currentPos >= encodeKey.Length)
                    currentPos = 0;
            }
        }
    }
}
