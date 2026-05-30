namespace CoreApp.Models
{
    public class Student : Person
    {
        public double GPA { get; set; }

        public override string ToString()
        {
            return base.ToString() + $", GPA: {GPA}";
        }
    }
}
