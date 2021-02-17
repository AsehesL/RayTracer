using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RayTracerNet
{
    public enum VectorPropertyType
    {
        Vector2,
        Vector3,
    }

    [AttributeUsage(AttributeTargets.Property, AllowMultiple = false, Inherited = false)]
    public class VectorPropertyDisplayAttribute : Attribute
    {
        public VectorPropertyType type;
        public string propertyName;

        public VectorPropertyDisplayAttribute(VectorPropertyType type, string propertyName)
        {
            this.type = type;
            this.propertyName = propertyName;
        }
    }

    [AttributeUsage(AttributeTargets.Property, AllowMultiple = false, Inherited = false)]
    public class NumberPropertyDisplayAttribute : Attribute
    {
        public double min;
        public double max;
        public string propertyName;

        public NumberPropertyDisplayAttribute(string propertyName)
        {
            this.propertyName = propertyName;
            min = double.MinValue;
            max = double.MaxValue;
        }
    }

    [AttributeUsage(AttributeTargets.Property, AllowMultiple = false, Inherited = false)]
    public class ColorPropertyDisplayAttribute : Attribute
    {
        public string propertyName;

        public ColorPropertyDisplayAttribute(string propertyName)
        {
            this.propertyName = propertyName;
        }
    }

    [AttributeUsage(AttributeTargets.Property, AllowMultiple = false, Inherited = false)]
    public class TextPropertyDisplayAttribute : Attribute
    {
        public string propertyName;

        public TextPropertyDisplayAttribute(string propertyName)
        {
            this.propertyName = propertyName;
        }
    }

    [AttributeUsage(AttributeTargets.Property, AllowMultiple = false, Inherited = false)]
    public class EnumPropertyDisplayAttribute : Attribute
    {
        public string propertyName;

        public EnumPropertyDisplayAttribute(string propertyName)
        {
            this.propertyName = propertyName;
        }
    }

    [AttributeUsage(AttributeTargets.Property, AllowMultiple = false, Inherited = false)]
    public class ResourcePropertyDisplayAttribute : Attribute
    {
        public string propertyName;

        public ResourcePropertyDisplayAttribute(string propertyName)
        {
            this.propertyName = propertyName;
        }
    }

    [AttributeUsage(AttributeTargets.Property, AllowMultiple = false, Inherited = false)]
    public class SwitchPropertyDisplayAttribute : Attribute
    {
        public string propertyName;

        public SwitchPropertyDisplayAttribute(string propertyName)
        {
            this.propertyName = propertyName;
        }
    }
}
