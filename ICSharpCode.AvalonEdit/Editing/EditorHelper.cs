using System;
using System.Linq;
using System.Text;
using System.Windows;
using System.Windows.Data;
using System.Windows.Media;
using System.Windows.Shapes;
using System.Collections.Generic;
using System.Windows.Controls;
using System.Windows.Documents;
using System.Windows.Input;
using System.Windows.Media.Imaging;
using System.Windows.Navigation;
using ICSharpCode.AvalonEdit.Document;
using ICSharpCode.AvalonEdit.Rendering;
using ICSharpCode.AvalonEdit.Utils;

namespace ICSharpCode.AvalonEdit.Editing
{
    /// <summary>
    /// Help the editor to input content
    /// </summary>
    public class EditorHelper
    {

        private TextEditor textEditor;
        bool[] CanEat;
        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="t"></param>
        public EditorHelper(TextEditor t)
        {
            textEditor = t;
            t.KeyUp += OnKeyUp;
            CanEat = new bool[ 10 ];
            for ( int i = 0; i < 10; i++ )
            {
                CanEat[ i ] = false;
            }
        }
        /// <summary>
        /// When you have inputted a charactor, it is called.
        /// </summary>
        /// <param name="sender"></param>
        /// <param name="kevent"></param>
        public void OnKeyUp( object sender, KeyEventArgs kevent )
        {
            switch ( kevent.Key )
            {
                case Key.OemOpenBrackets:
                    //{
                    if ( Keyboard.IsKeyDown( Key.LeftShift ) || Keyboard.IsKeyDown( Key.RightShift ) )
                    {
                        int tabnum = CountTab();
                        string s = "\n";
                        for ( int i = 0; i < tabnum; i++ )
                        {
                            s+="\t";
                        }
                        s += "\n";
                        for ( int i = 0; i < tabnum - 1; i++ )
                        {
                            s+="\t";
                        }
                        s += "}";
                        
                        textEditor.Document.Insert( textEditor.CaretOffset, s );
                        for ( int i = 0; i < tabnum+1; i++ )
                        {
                            if ( textEditor.CaretOffset  > 0 )
                            {
                                textEditor.CaretOffset--;
                            }

                        }
                        CanEat[ 0 ] = true;
                    }
                    else// [
                    {
                        textEditor.Document.Insert( textEditor.CaretOffset, "]" );
                        textEditor.CaretOffset--;
                        CanEat[ 1 ] = true;
                    }
                    break;


                case Key.OemCloseBrackets:
                    //}
                    if ( Keyboard.IsKeyDown( Key.LeftShift ) || Keyboard.IsKeyDown( Key.RightShift ) )
                    {
                        if ( CanEat[ 0 ] )
                        {
                            CanEat[ 0 ] = false;
                            textEditor.Document.Remove( textEditor.CaretOffset-1, 1 );
                        }
                    }
                    else// ]
                    {
                        if ( CanEat[ 1 ] )
                        {
                            CanEat[ 1 ] = false;
                            textEditor.Document.Remove( textEditor.CaretOffset-1, 1 );
                        }

                    }
                    break;


                case Key.D9:
                    //(
                    if ( Keyboard.IsKeyDown( Key.LeftShift ) || Keyboard.IsKeyDown( Key.RightShift ) )
                    {
                        textEditor.Document.Insert( textEditor.CaretOffset, ")" );
                        textEditor.CaretOffset--;
                        CanEat[ 2 ] = true;
                    }
                    break;

                case Key.D0:
                    //}
                    if ( Keyboard.IsKeyDown( Key.LeftShift ) || Keyboard.IsKeyDown( Key.RightShift ) )
                    {
                        if ( CanEat[ 2 ] )
                        {
                            CanEat[ 2 ] = false;
                            textEditor.Document.Remove( textEditor.CaretOffset-1, 1 );
                        }
                    }
                    break;
                    

            }

            
        }


        /// <summary>
        /// count how many tab must draw to make the editor looked well
        /// </summary>
        /// <returns></returns>
       private int CountTab()
       {
            string text = textEditor.TextArea.TextView.Document.GetText( 0, textEditor.CaretOffset );
            int tableNum = 0;
            foreach ( char c in text )
            {
                if (c == '{')
                {
                    tableNum++;
                }
                else if (c == '}')
                {
                    tableNum--;
                }
            }
            return tableNum;
        }

    }
}
