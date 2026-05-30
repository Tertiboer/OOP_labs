using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Windows;
using CoreApp.Models;
using CoreApp.Plugins;

namespace CoreApp
{
    /// <summary>
    /// Main application window.
    /// </summary>
    public partial class MainWindow : Window
    {
        private readonly List<Person> objects = new();

        private readonly Dictionary<string, Func<Person>> creators =
            new Dictionary<string, Func<Person>>();

        public MainWindow()
        {
            InitializeComponent();

            RegisterBuiltInTypes();

            LoadPlugins();

            RefreshComboBox();
        }

        /// <summary>
        /// Registers internal application classes.
        /// </summary>
        private void RegisterBuiltInTypes()
        {
            creators["Student"] = () => new Student
            {
                GPA = 8.5
            };
        }

        /// <summary>
        /// Dynamically loads plugins from folder.
        /// </summary>
        private void LoadPlugins()
        {
            string pluginFolder = Path.Combine(
                AppDomain.CurrentDomain.BaseDirectory,
                "Plugins");

            if (!Directory.Exists(pluginFolder))
            {
                Directory.CreateDirectory(pluginFolder);
            }

            string[] dllFiles = Directory.GetFiles(pluginFolder, "*.dll");

            foreach (string file in dllFiles)
            {
                try
                {
                    Assembly assembly = Assembly.LoadFrom(file);

                    IEnumerable<Type> pluginTypes = assembly
                        .GetTypes()
                        .Where(t => typeof(IPlugin).IsAssignableFrom(t)
                                 && !t.IsInterface
                                 && !t.IsAbstract);

                    foreach (Type type in pluginTypes)
                    {
                        IPlugin plugin =
                            (IPlugin)Activator.CreateInstance(type)!;

                        Dictionary<string, Func<Person>> pluginTypesMap =
                            plugin.RegisterTypes();

                        foreach (var pair in pluginTypesMap)
                        {
                            creators[pair.Key] = pair.Value;
                        }

                        MessageBox.Show(
                            $"Plugin loaded: {plugin.PluginName}");
                    }
                }
                catch (Exception ex)
                {
                    MessageBox.Show(ex.Message);
                }
            }
        }

        /// <summary>
        /// Refreshes available class list.
        /// </summary>
        private void RefreshComboBox()
        {
            TypeComboBox.ItemsSource = creators.Keys.ToList();

            TypeComboBox.SelectedIndex = 0;
        }

        /// <summary>
        /// Creates new object.
        /// </summary>
        private void Add_Click(object sender, RoutedEventArgs e)
        {
            try
            {
                string selectedType =
                    TypeComboBox.SelectedItem.ToString()!;

                Person person = creators[selectedType]();

                person.Name = NameTextBox.Text;

                person.Age = int.Parse(AgeTextBox.Text);

                objects.Add(person);

                RefreshList();
            }
            catch (Exception ex)
            {
                MessageBox.Show(ex.Message);
            }
        }

        /// <summary>
        /// Updates object list UI.
        /// </summary>
        private void RefreshList()
        {
            ObjectsListBox.ItemsSource = null;

            ObjectsListBox.ItemsSource = objects;
        }
    }
}
