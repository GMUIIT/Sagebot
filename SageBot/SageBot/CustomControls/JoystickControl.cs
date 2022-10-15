using System;
using System.Collections.Generic;
using System.Text;
using Xamarin.Forms;

namespace SageBot.CustomControls
{
    public class JoystickControl : View
    {
        // Updating the X text displayed on the app.
        public static readonly BindableProperty XpositionProperty =
            BindableProperty.Create(
                propertyName: "Xposition",
                returnType: typeof(int),
                declaringType: typeof(int),
                defaultValue: 0
            );

        // Gets the X position of the joystick.
        public int Xposition
        {
            get { return (int)GetValue(XpositionProperty); }
            set { SetValue(XpositionProperty, value); }
        }

        // Updating the Y text displayed on the app.
        public static readonly BindableProperty YpositionProperty =
            BindableProperty.Create(
                propertyName: "Yposition",
                returnType: typeof(int),
                declaringType: typeof(int),
                defaultValue: 0
            );

        // Gets the Y position of the joystick.
        public int Yposition
        {
            get { return (int)GetValue(YpositionProperty); }
            set { SetValue(YpositionProperty, value); }
        }

        // Updating the distance text displayed on the app.
        public static readonly BindableProperty DistanceProperty =
            BindableProperty.Create(
                propertyName: "Distance",
                returnType: typeof(double),
                declaringType: typeof(double),
                defaultValue: 0.0
            );

        // Distance from the origin to the outer radius from 0-100.
        // Conversion to speed: (Distance/100 * 45) + 200
        public double Distance
        {
            get { return Math.Round((double)GetValue(DistanceProperty)); }
            set { SetValue(DistanceProperty, value); }
        }

        // Updating the distance text displayed on the app.
        public static readonly BindableProperty AngleProperty =
            BindableProperty.Create(
                propertyName: "Angle",
                returnType: typeof(double),
                declaringType: typeof(double),
                defaultValue: 0.0
            );

        // Distance from the origin to the outer radius from 0-360.
        public double Angle
        {
            get { return Math.Round((double)GetValue(AngleProperty), 2); }
            set { SetValue(AngleProperty, value); }
        }
    }
}
