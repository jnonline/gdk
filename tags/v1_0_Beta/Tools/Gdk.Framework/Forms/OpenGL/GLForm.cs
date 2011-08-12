/*
  Copyright (c) 2011, Raincity Games LLC
  Licensed under the MIT license: http://www.opensource.org/licenses/mit_license.php
*/


using System;
using System.Windows.Forms;
using System.Runtime.InteropServices;

using Tao.OpenGl;
using Tao.Platform.Windows;

namespace Gdk.Forms
{
    public class GLForm : Form
    {
        public IntPtr hRC = IntPtr.Zero;
        public IntPtr hDC = IntPtr.Zero;

        /// <summary>
        /// CTor
        /// </summary>
        public GLForm()
        {
            // Setup the basic form details
            this.ClientSize = new System.Drawing.Size(256, 256);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.FixedDialog;
            this.MaximizeBox = false;
            this.MinimizeBox = false;
            this.Name = "GLForm";
            this.ShowIcon = false;
            this.ShowInTaskbar = false;
            this.Text = "GL Utility Window";
            this.TopMost = true;
            this.Visible = false;
        }

        /// <summary>
        /// Called when the window is first created
        /// </summary>
        protected override void OnCreateControl()
        {
            // Call the base
            base.OnCreateControl();

            // Initialize GL
            InitGL();
        }

        /// <summary>
        /// Called when the form is closing
        /// </summary>
        /// <param name="e"></param>
        protected override void OnClosing(System.ComponentModel.CancelEventArgs e)
        {
            // Release the GL context
            if (hRC != IntPtr.Zero)
            {
                // Unselect the current rendering context
                Wgl.wglMakeCurrent(IntPtr.Zero, IntPtr.Zero);

                // Delete the RC
                Wgl.wglDeleteContext(hRC);

                hRC = IntPtr.Zero;
            }

            // Release the DC
            if (hDC != IntPtr.Zero)
            {                        
                // Is it safe to release the DC?
                if (!this.IsDisposed && this.Handle != IntPtr.Zero)
                {
                    User.ReleaseDC(this.Handle, hDC);
                }

                hDC = IntPtr.Zero;
            }

            base.OnClosing(e);
        }

        /// <summary>
        /// Initialized OpenGL & the appropriate contexts
        /// </summary>
        public unsafe void InitGL()
        {
            // Bail if we're already initialized
            if (hRC != IntPtr.Zero)
                return;

            // Create a pixel format that supports OpenGL
            Gdi.PIXELFORMATDESCRIPTOR pfd = new Gdi.PIXELFORMATDESCRIPTOR();
            pfd.nSize = (short) Marshal.SizeOf(pfd); // should be 40  // sizeof(PIXELFORMATDESCRIPTOR); 
            pfd.nVersion = 1;
            pfd.dwFlags = (Gdi.PFD_DRAW_TO_WINDOW | Gdi.PFD_SUPPORT_OPENGL | Gdi.PFD_DOUBLEBUFFER);
            pfd.iPixelType = (byte)(Gdi.PFD_TYPE_RGBA);
            pfd.cColorBits = 32;
            pfd.cRedBits = 0;
            pfd.cRedShift = 0;
            pfd.cGreenBits = 0;
            pfd.cGreenShift = 0;
            pfd.cBlueBits = 0;
            pfd.cBlueShift = 0;
            pfd.cAlphaBits = 0;
            pfd.cAlphaShift = 0;
            pfd.cAccumBits = 0;
            pfd.cAccumRedBits = 0;
            pfd.cAccumGreenBits = 0;
            pfd.cAccumBlueBits = 0;
            pfd.cAccumAlphaBits = 0;
            pfd.cDepthBits = 16;
            pfd.cStencilBits = 0;
            pfd.cAuxBuffers = 0;
            pfd.iLayerType = (byte)(Gdi.PFD_MAIN_PLANE);
            pfd.bReserved = 0;
            pfd.dwLayerMask = 0;
            pfd.dwVisibleMask = 0;
            pfd.dwDamageMask = 0;

            // Get the DC of this window
            hDC = User.GetDC(this.Handle);

            // Choose the Pixel Format
            int pixelFormat = Gdi.ChoosePixelFormat(hDC, ref pfd);
            if (pixelFormat == 0)
            {
                uint error = GetLastError();
                throw new ApplicationException("ChoosePixelFormat() Error: " + error);
            }

            // Set Pixel Format
            if (Gdi.SetPixelFormat(hDC, pixelFormat, ref pfd) == false)
            {
                uint error = GetLastError();
                throw new ApplicationException("SetPixelFormat() Error: " + error);
            }

            // Create the GL Render Context
            hRC = Wgl.wglCreateContext(hDC);
            if (hRC == IntPtr.Zero)
            {
                uint error = GetLastError();
                throw new ApplicationException("wglCreateContext() Error: " + error);
            }
            
            // Use this render context
            MakeRenderContextCurrent();
        }

        /// <summary>
        /// Makes the render context of this window, active
        /// </summary>
        public void MakeRenderContextCurrent()
        {
            // Make the render context current
            if (Wgl.wglMakeCurrent(hDC, hRC) == false)
            {
                uint error = GetLastError();
                throw new ApplicationException("wglMakeCurrent() Error: " + error);
            }
        }

        // ============================================================================
        // DWORD GetLastError(VOID);
        // ============================================================================
        [DllImport("kernel32")]
        public static extern unsafe
            uint GetLastError();
    } 
} 