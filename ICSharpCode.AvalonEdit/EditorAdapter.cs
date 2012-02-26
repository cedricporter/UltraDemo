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
    /// Provide the interface of the TextEditor
    /// </summary>
    public class EditorAdapter
    {
        private TextEditor textEditor;
        private BreakPointMargin bkM;
        private EditorHelper editorHelper;

        /// <summary>
        /// Constructor
        /// </summary>
        /// <param name="textEditor"></param>
        public EditorAdapter( TextEditor textEditor )
        {
            this.textEditor =  textEditor;
            editorHelper = new EditorHelper( textEditor );
            bkM = (BreakPointMargin)textEditor.TextArea.LeftMargins[ 0 ];
        }

        /// <summary>
        /// Get the height of a line
        /// </summary>
        /// <returns></returns>
        public double getLineHeight()
        {
            return bkM.LineHeight;
        }

        /// <summary>
        /// Get current editing line number
        /// </summary>
        /// <param name="p"></param>
        /// <returns></returns>
        public int getCurrentLineNum( Point p )
        {
            return (int)( ( textEditor.VerticalOffset + p.Y ) / getLineHeight() ) + 1;
        }

        /// <summary>
        /// Get the break point list
        /// </summary>
        /// <returns></returns>
        public List<int> getBreakPointList()
        {
            return bkM.GetBreakPointList();
        }

        /// <summary>
        /// Get the text in the editor
        /// </summary>
        /// <returns></returns>
        public string GetAllText()
        {
            return textEditor.Text;
        }

        /// <summary>
        /// Get the text in a concrete line
        /// </summary>
        /// <param name="LineNum"></param>
        /// <returns></returns>
        public string GetTextByLineNum( int LineNum )
        {
            if ( LineNum > textEditor.LineCount )
                return "";
            int offset = textEditor.TextArea.TextView.Document.GetLineByNumber( LineNum ).Offset;
            int length = textEditor.TextArea.TextView.Document.GetLineByNumber( LineNum ).Length;
            return textEditor.TextArea.TextView.Document.GetText( offset, length );
        }


        /// <summary>
        /// Selet one line
        /// </summary>
        /// <param name="LineNum"></param>
        /// <returns></returns>
        public bool SelectOneLine( int LineNum )
        {
            if ( LineNum > textEditor.LineCount )
                return false;
            int minVline = (int)( ( textEditor.VerticalOffset ) / getLineHeight() ) + 1;
            int maxVline = minVline + textEditor.LineCount;
            if ( LineNum > maxVline || LineNum < minVline )
            {
                textEditor.ScrollToLine( LineNum );
            }
            int offset = textEditor.TextArea.TextView.Document.GetLineByNumber( LineNum ).Offset;
            int length = textEditor.TextArea.TextView.Document.GetLineByNumber( LineNum ).Length;
            textEditor.Select( offset, length );
            return true;
        }

        /// <summary>
        /// Present a line by highlight the background, but can show a line that not exist
        /// </summary>
        /// <param name="IsShow"></param>
        /// <param name="LineNum"></param>
        /// <param name="c"></param>
        public void ShowLine( bool IsShow, int LineNum, Color c )
        {
            if (LineNum > textEditor.LineCount)
            {
                return;
            }
            int minVline = (int)( ( textEditor.VerticalOffset) / getLineHeight() ) + 1;
            int maxVline = minVline + textEditor.LineCount;
            if (LineNum > maxVline || LineNum < minVline)
            {
                textEditor.ScrollToLine( LineNum );
            }
            textEditor.TextArea.TextView.ShowLine( IsShow, LineNum, c );
        }




    }

    
}
