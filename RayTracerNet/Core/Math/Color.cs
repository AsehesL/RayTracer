using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace RayTracerNet
{
    public struct Color
    {
        public float r, g, b, a;

        public float grayScale
        {
            get { return 0.299f * r + 0.587f * g + 0.114f * b; }
        }

        public static Color black
        {
            get { return new Color(0, 0, 0, 1); }
        }

        public static Color white
        {
            get { return new Color(1, 1, 1, 1); }
        }

        public override string ToString()
        {
            return $"({r}, {g}, {b}, {a})";
        }

        public float this[int index]
        {
            get
            {
                if (index == 0)
                    return r;
                else if (index == 1)
                    return g;
                else if (index == 2)
                    return b;
                else if (index == 3)
                    return a;
                throw new System.IndexOutOfRangeException();
            }
            set
            {
                if (index == 0)
                    r = value;
                else if (index == 1)
                    g = value;
                else if (index == 2)
                    b = value;
                else if (index == 3)
                    a = value;
                throw new System.IndexOutOfRangeException();
            }
        }

        public Color(float r, float g, float b, float a)
        {
            this.r = r;
            this.g = g;
            this.b = b;
            this.a = a;
        }

        public Color(float r, float g, float b)
        {
            this.r = r;
            this.g = g;
            this.b = b;
            this.a = 1.0f;
        }

        public void ToHSV(ref float h, ref float s, ref float v)
        {
            Color.RGBToHSV(this, ref h, ref s, ref v);
        }

        public float Luminance()
        {
            return r * 0.22f + g * 0.707f + b * 0.071f;
        }

        //public void FixColor(float gamma)
        //{
        //    this.r = (float)Math.Pow(MathUtils.SaturateF(r), gamma);
        //    this.g = (float)Math.Pow(MathUtils.SaturateF(g), gamma);
        //    this.b = (float)Math.Pow(MathUtils.SaturateF(b), gamma);
        //    this.a = (float)Math.Pow(MathUtils.SaturateF(a), gamma);
        //}

        public void FixColor()
        {
            this.r = (float)MathUtils.SaturateF(r);
            this.g = (float)MathUtils.SaturateF(g);
            this.b = (float)MathUtils.SaturateF(b);
            this.a = (float)MathUtils.SaturateF(a);
        }

        public void Tonemapping(float exposure)
        {
            this.r = TonemappingChannel(this.r, exposure);
            this.g = TonemappingChannel(this.g, exposure);
            this.b = TonemappingChannel(this.b, exposure);
            this.a = TonemappingChannel(this.a, exposure);
        }

        private float TonemappingChannel(float channel, float exposure)
        {
            channel *= exposure;
            return (channel * (2.51f * channel + 0.03f)) / (channel * (2.43f * channel + 0.59f) + 0.14f);
        }

        public void Gamma(float gamma)
        {
            this.r = (float)Math.Pow(r, gamma);
            this.g = (float)Math.Pow(g, gamma);
            this.b = (float)Math.Pow(b, gamma);
            this.a = (float)Math.Pow(a, gamma);
        }

        public System.Drawing.Color ToSystemColor()
        {
            int r8 = (int)(this.r * 255.0f);
            int g8 = (int)(this.g * 255.0f);
            int b8 = (int)(this.b * 255.0f);
            int a8 = (int)(this.a * 255.0f);
            if (r8 < 0)
                r8 = 0;
            if (r8 > 255)
                r8 = 255;
            if (g8 < 0)
                g8 = 0;
            if (g8 > 255)
                g8 = 255;
            if (b8 < 0)
                b8 = 0;
            if (b8 > 255)
                b8 = 255;
            if (a8 < 0)
                a8 = 0;
            if (a8 > 255)
                a8 = 255;
            System.Drawing.Color col = System.Drawing.Color.FromArgb(a8, r8, g8, b8);
            return col;
        }

        public static Color operator +(Color a, Color b)
        {
            return new Color(a.r + b.r, a.g + b.g, a.b + b.b, a.a + b.a);
        }

        public static Color operator -(Color a, Color b)
        {
            return new Color(a.r - b.r, a.g - b.g, a.b - b.b, a.a - b.a);
        }

        public static Color operator *(Color a, Color b)
        {
            return new Color(a.r * b.r, a.g * b.g, a.b * b.b, a.a * b.a);
        }

        public static Color operator *(Color color, float scale)
        {
            return new Color(color.r * scale, color.g * scale, color.b * scale, color.a * scale);
        }

        public static Color operator *(float scale, Color color)
        {
            return new Color(color.r * scale, color.g * scale, color.b * scale, color.a * scale);
        }

        public static Color operator /(Color a, Color b)
        {
            return new Color(a.r / b.r, a.g / b.g, a.b / b.b, a.a / b.a);
        }

        public static Color operator /(Color color, float scale)
        {
            float invS = 1.0f / scale;
            return new Color(color.r * invS, color.g * invS, color.b * invS, color.a * invS);
        }

        public static bool operator ==(Color a, Color b)
        {
            if (a.r - b.r > float.Epsilon)
                return false;
            if (b.r - a.r > float.Epsilon)
                return false;
            if (a.g - b.g > float.Epsilon)
                return false;
            if (b.g - a.g > float.Epsilon)
                return false;
            if (a.b - b.b > float.Epsilon)
                return false;
            if (b.b - a.b > float.Epsilon)
                return false;
            if (a.a - b.a > float.Epsilon)
                return false;
            if (b.a - a.a > float.Epsilon)
                return false;
            return true;
        }

        public static bool operator !=(Color a, Color b)
        {
            if (a.r - b.r > float.Epsilon)
                return true;
            if (b.r - a.r > float.Epsilon)
                return true;
            if (a.g - b.g > float.Epsilon)
                return true;
            if (b.g - a.g > float.Epsilon)
                return true;
            if (a.b - b.b > float.Epsilon)
                return true;
            if (b.b - a.b > float.Epsilon)
                return true;
            if (a.a - b.a > float.Epsilon)
                return true;
            if (b.a - a.a > float.Epsilon)
                return true;
            return false;
        }

        public override bool Equals(object obj)
        {
            Color objcol = (Color)obj;
            return this == objcol;
        }

        public static Color Lerp(Color a, Color b, float t)
        {
            return new Color(a.r + (b.r - a.r) * t, a.g + (b.g - a.g) * t, a.b + (b.b - a.b) * t, a.a + (b.a - a.a) * t);
        }

        public static Color HSVToRGB(float h, float s, float v)
        {
            Color col = new Color(1.0f, 1.0f, 1.0f, 1.0f);
            if (s >= 0.0f - float.Epsilon && s <= 0.0f + float.Epsilon)
            {
                col.r = v;
                col.g = v;
                col.b = v;
            }
            else if (v >= 0.0f - float.Epsilon && v <= 0.0f + float.Epsilon)
            {
                col.r = 0.0f;
                col.g = 0.0f;
                col.b = 0.0f;
            }
            else
            {
                col.r = 0.0f;
                col.g = 0.0f;
                col.b = 0.0f;
                float num = h * 6.0f;
                int num2 = (int)Math.Floor(num);
                float num3 = num - (float)num2;
                float num4 = v * (1.0f - s);
                float num5 = v * (1.0f - s * num3);
                float num6 = v * (1.0f - s * (1.0f - num3));
                switch (num2 + 1)
                {
                    case 0:
                        col.r = v;
                        col.g = num4;
                        col.b = num5;
                        break;
                    case 1:
                        col.r = v;
                        col.g = num6;
                        col.b = num4;
                        break;
                    case 2:
                        col.r = num5;
                        col.g = v;
                        col.b = num4;
                        break;
                    case 3:
                        col.r = num4;
                        col.g = v;
                        col.b = num6;
                        break;
                    case 4:
                        col.r = num4;
                        col.g = num5;
                        col.b = v;
                        break;
                    case 5:
                        col.r = num6;
                        col.g = num4;
                        col.b = v;
                        break;
                    case 6:
                        col.r = v;
                        col.g = num4;
                        col.b = num5;
                        break;
                    case 7:
                        col.r = v;
                        col.g = num6;
                        col.b = num4;
                        break;
                }
            }
            return col;
        }

        public static Color Color32(ushort r, ushort g, ushort b, ushort a)
        {
            float red = ((float)r) / 255.0f;
            float green = ((float)g) / 255.0f;
            float blue = ((float)b) / 255.0f;
            float alpha = ((float)a) / 255.0f;
            return new Color(red, green, blue, alpha);
        }

        public static Color Color32(ushort r, ushort g, ushort b)
        {
            float red = ((float)r) / 255.0f;
            float green = ((float)g) / 255.0f;
            float blue = ((float)b) / 255.0f;
            return new Color(red, green, blue, 1.0f);
        }

        public static void RGBToHSV(Color color, ref float h, ref float s, ref float v)
        {
            if (color.b > color.g && color.b > color.r)
            {
                Color.RGBToHSV_Internal(4.0f, color.b, color.r, color.g, ref h, ref s, ref v);
            }
            else if (color.g > color.r)
            {
                Color.RGBToHSV_Internal(2.0f, color.g, color.b, color.r, ref h, ref s, ref v);
            }
            else
            {
                Color.RGBToHSV_Internal(0.0f, color.r, color.g, color.b, ref h, ref s, ref v);
            }
        }

        public static Color Parse(string text)
        {
            if (string.IsNullOrEmpty(text))
                return Color.black;
            string[] split = text.Split(',');

            float r = split.Length >= 1 ? float.Parse(split[0]) : 0.0f;
            float g = split.Length >= 2 ? float.Parse(split[1]) : 0.0f;
            float b = split.Length >= 3 ? float.Parse(split[2]) : 0.0f;
            float a = split.Length >= 4 ? float.Parse(split[3]) : 1.0f;

            Color color = new Color(r, g, b, a);
            color.Gamma(2.2f);
            return color;
        }

        private static void RGBToHSV_Internal(float offset, float dominantcolor, float colorone, float colortwo, ref float h, ref float s, ref float v)
        {
            v = dominantcolor;
            if (v < 0.0f - float.Epsilon || v > 0.0f + float.Epsilon)
            {
                float num;
                if (colorone > colortwo)
                {
                    num = colortwo;
                }
                else
                {
                    num = colorone;
                }
                float num2 = v - num;
                if (num2 < 0.0f - float.Epsilon || num2 > 0.0f + float.Epsilon)
                {
                    s = num2 / v;
                    h = offset + (colorone - colortwo) / num2;
                }
                else
                {
                    s = 0.0f;
                    h = offset + (colorone - colortwo);
                }
                h /= 6.0f;
                if (h < 0.0f)
                {
                    h += 1.0f;
                }
            }
            else
            {
                s = 0.0f;
                h = 0.0f;
            }
        }
    }
}
