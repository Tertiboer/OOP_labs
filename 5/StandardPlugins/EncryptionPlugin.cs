using System;
using System.Collections.Generic;
using PluginInterface;

namespace StandardPlugins
{
    /// <summary>
    /// Plugin that provides basic XOR encryption data transformation.
    /// </summary>
    public class EncryptionPlugin : IPlugin
    {
        public string Name => "XOR Encryption Plugin";
        public string Description => "Encrypts/Decrypts data using a customizable XOR key.";
        public Dictionary<string, string> Settings { get; set; } = new Dictionary<string, string>();

        public EncryptionPlugin()
        {
            // Default configuration parameter (10-point task requirement)
            Settings["Key"] = "42";
        }

        public byte[] ProcessBeforeSave(byte[] inputData)
        {
            return ApplyXor(inputData);
        }

        public byte[] ProcessAfterLoad(byte[] inputData)
        {
            return ApplyXor(inputData); // XOR is symmetric
        }

        public void Configure()
        {
            Console.WriteLine($"\n--- [{Name}] Settings ---");
            Console.Write($"Enter new numeric XOR key (Current: {Settings["Key"]}): ");
            string input = Console.ReadLine();
            if (byte.TryParse(input, out _))
            {
                Settings["Key"] = input;
                Console.WriteLine("Key updated successfully!");
            }
            else
            {
                Console.WriteLine("Invalid input. Keeping previous key.");
            }
        }

        /// <summary>
        /// Performs the core XOR mathematical transformation on byte arrays.
        /// </summary>
        private byte[] ApplyXor(byte[] data)
        {
            byte key = byte.Parse(Settings["Key"]);
            byte[] result = new byte[data.Length];
            for (int i = 0; i < data.Length; i++)
            {
                result[i] = (byte)(data[i] ^ key);
            }
            return result;
        }
    }
}