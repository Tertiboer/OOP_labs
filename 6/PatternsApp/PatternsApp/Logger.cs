
using System;

namespace PatternsApp
{
    // Singleton pattern
    public class Logger
    {
        private static Logger _instance;

        public static Logger Instance
        {
            get
            {
                if (_instance == null)
                    _instance = new Logger();
                return _instance;
            }
        }

        private Logger() {}

        public void Log(string message)
        {
            Console.WriteLine("[LOG]: " + message);
        }
    }
}
