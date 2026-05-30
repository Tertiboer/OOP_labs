namespace CoreApp.Models
{
    /// <summary>
    /// Base abstract class.
    /// </summary>
    public abstract class Person
    {
        public string Name { get; set; } = "";

        public int Age { get; set; }

        public override string ToString()
        {
            return $"{GetType().Name}: {Name}, Age: {Age}";
        }
    }
}
