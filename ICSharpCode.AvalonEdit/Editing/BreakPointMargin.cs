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


    class BreakPointMargin : Canvas
    {

        public double LineHeight;
        public TextEditor TxEditor;
        public List<int> BreakPointList;



 
        public BreakPointMargin(TextEditor TxEditor)
        {
            this.TxEditor = TxEditor;
            BreakPointList = new List<int>();
            this.HorizontalAlignment = HorizontalAlignment.Left;
            this.VerticalAlignment = VerticalAlignment.Top;
            this.TxEditor.TextArea.TextView.VisualLinesChanged += OnVisualLinesChanged;
            this.RenderSize = new System.Windows.Size( 20, 100 );

            this.Height = 500;
            this.Width = 20;

            this.Background = Brushes.Silver;
            
        }

        public void AddaBreakPoint(int LineNum)
        {
            BreakPointList.Add( LineNum );
        }

        public void RemoveaBreakPoint( int LineNum )
        {
            BreakPointList.Remove( LineNum );
        }


        public int getCurrentLineNum()
        {
            Point p = Mouse.GetPosition( this );
            return (int)( (TxEditor.VerticalOffset + p.Y) / LineHeight ) + 1;
        }

        protected override void OnMouseLeftButtonDown( MouseButtonEventArgs e )
        {
            Point p = Mouse.GetPosition( this );
            int lineNum = getCurrentLineNum();
            if ( TxEditor.LineCount < lineNum )
                return;
            foreach (int l in BreakPointList)
            {
                if (l == lineNum)
                {
                    RemoveaBreakPoint( l );
                    InvalidateVisual();
                    return;
                }
            }
            AddaBreakPoint( getCurrentLineNum() );
            InvalidateVisual();
        }


        private void OnVisualLinesChanged( object sender, EventArgs e )
        {
            InvalidateVisual();
        }

        public List<int> GetBreakPointList()
        {
            return BreakPointList;
        }

        /// <inheritdoc/>
        protected override void OnRender( DrawingContext drawingContext )
        {
            base.OnRender( drawingContext );
            Brush SolidBrush = Brushes.Red;
            Pen pen = new Pen();
            TxEditor.TextArea.TextView.EnsureVisualLines();
            foreach ( VisualLine line in TxEditor.TextArea.TextView.VisualLines )
            {
                this.LineHeight = line.Height;
                foreach (int n in BreakPointList)
                {
                    if( n == line.FirstDocumentLine.LineNumber )
                        drawingContext.DrawEllipse( SolidBrush, pen, new Point( 10, line.VisualTop - TxEditor.TextArea.TextView.VerticalOffset + LineHeight / 2), 5, 5 );
                }
            }
        }

		
	}
    
}
