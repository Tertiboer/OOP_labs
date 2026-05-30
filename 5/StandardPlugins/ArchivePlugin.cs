using System;
using System.Collections.Generic;
using System.IO;
using System.IO.Compression;
using PluginInterface;

namespace StandardPlugins
{
    /// <summary>
    /// Plugin that handles Deflate compression/decompression algorithms.
    /// </summary>
    public class ArchivePlugin : IPlugin
    {
        public string Name => "Deflate Compression Plugin";
        public string Description => "Compresses data before saving and decompresses on load.";
        public Dictionary<string, string> Settings { get; set; } = new Dictionary<string, string>();

        public ArchivePlugin()
        {
            Settings["CompressionLevel"] = "Optimal"; // Options: Optimal, Fastest
        }

        public byte[] ProcessBeforeSave(byte[] inputData)
        {
            using (var outputStream = new MemoryStream())
            {
                var level = Settings["CompressionLevel"] == "Fastest" ? CompressionLevel.Fastest : CompressionLevel.Optimal;
                using (var deflateStream = new DeflateStream(outputStream, level))
                {
                    deflateStream.Write(inputData, 0, inputData.Length);
                }
                return outputStream.ToArray();
            }
        }

        public byte[] ProcessAfterLoad(byte[] inputData)
        {
            using (var inputStream = new MemoryStream(inputData))
            using (var deflateStream = new DeflateStream(inputStream, CompressionMode.Decompress))
            using (var outputStream = new MemoryStream())
            {
                deflateStream.CopyTo(outputStream);
                return outputStream.ToArray();
            }
        }

        public void Configure()
        {
            Console.WriteLine($"\n--- [{Name}] Settings ---");
            Console.WriteLine("Select Compression Level:");
            Console.WriteLine("1. Optimal");
            Console.WriteLine("2. Fastest");
            Console.Write($"Choice (Current: {Settings["CompressionLevel"]}): ");

            string choice = Console.ReadLine();
            if (choice == "1") Settings["CompressionLevel"] = "Optimal";
            if (choice == "2") Settings["CompressionLevel"] = "Fastest";
            Console.WriteLine($"Compression level set to: {Settings["CompressionLevel"]}");
        }
    }
}