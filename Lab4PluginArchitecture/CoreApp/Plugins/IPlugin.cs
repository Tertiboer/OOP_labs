using System;
using System.Collections.Generic;
using CoreApp.Models;

namespace CoreApp.Plugins
{
    /// <summary>
    /// Plugin contract interface.
    /// </summary>
    public interface IPlugin
    {
        string PluginName { get; }

        Dictionary<string, Func<Person>> RegisterTypes();
    }
}
