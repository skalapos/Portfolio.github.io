"""
 * @file stegMain.py
 * @author Steven Kalapos 
 * @date January 2018
 * @modified Feb 19 2018
 * @brief contains DCT steg GUI and frontend funtions
 */
 """

from DCT import DCT
import tkinter as tk
from tkinter import messagebox, filedialog
import tkinter.ttk as ttk
import tkinter.scrolledtext as tkst
from PIL import Image, ImageTk
import os

root = tk.Tk()
root.resizable(0,0)

endImage = None
startImage = None

#GENERAL FUNCTIONS

def exitFunc(event):
    result = tk.messagebox.askyesno("Exit", "Do you wish to Exit?")
    if result == True:
        global quit 
        quit = 1

def openFunc(event):
    tempFileName = filedialog.askopenfilename(initialdir= '/',title = 'Select File', filetypes = (("PNG Files","*.png"),("JPEG Files","*.jpg"),("all files","*.*")))
    
    if tempFileName:
        return tempFileName  
    else:
        return None

#ENCRYPT FUNCTIONS
def goBackEn(event):
    global encryptScreen

    encryptScreen.pack_forget()

    openingScreen()

def encryptWindow():
    global startScreen
    global encryptScreen
    global encryptImgFrame
    global imageFrame
    global newImgFrame
    global inputBox

    root.title("Encrypt -- Image Hider v2.2.3")
    root.geometry('{}x{}'.format(750,650))
    startScreen.pack_forget()
    
    encryptScreen = tk.Frame(root, height =600, width = 750)
    encryptScreen.pack(expand = False)

    #make frames for encryption screen
    imageFrame = tk.Frame(encryptScreen, height = 500, width = 750,bg='blue')
    imageFrame.pack(fill='x', expand = True)

    messageFrame = tk.Frame(encryptScreen, height = 50, width = 750, bg='black')
    messageFrame.pack(fill='x', expand = True)

    buttonFrame = tk.Frame(encryptScreen, height = 50,width = 750)
    buttonFrame.pack(fill ='both', expand = True)

    ### making all buttons
    fileChooseButton = tk.Button(buttonFrame, text="Choose File...", command=lambda:chooseFileFnE(None))
    fileChooseButton.pack(side='left', padx = 77)

    eButton = tk.Button(buttonFrame, text="Encrypt", command=lambda:encryptFn(None))
    eButton.pack(side='left', padx = 77)

    backButton = tk.Button(buttonFrame, text="Back", command=lambda:goBackEn(None))
    backButton.pack(side='left', padx=77)

    ##make image windows
    encryptImgFrame = tk.Frame(imageFrame, height=450, width = 300, relief = 'ridge')
    encryptImgFrame.pack(side = 'left', padx=25, pady=25)

    newImgFrame = tk.Frame(imageFrame, height=450, width= 300, relief='ridge')
    newImgFrame.pack(side='right', padx=25, pady=25)

    #make encryption message input box
    inputBox = tk.Entry(messageFrame, relief = 'ridge', width = 250)
    inputBox.pack(padx = 25, pady =25)

def chooseFileFnE(event):
    global inFile
    global encryptScreen
    global encryptImgFrame
    global newImgFrame
    global endImage


    inFile = openFunc(None)
    
    if inFile == None:
        print("Encryption Error: File Not Opened!")
        return

    #convert jpeg to png if needed
    if inFile[-4:]==".jpg":
        im = Image.open(inFile)
        im.save(inFile[:-4]+".png")
        inFile = (inFile[:-4]+".png")    
    
    #clean image displays
    encryptImgFrame.pack_forget()
    newImgFrame.pack_forget()
    if endImage:
        endImage.pack_forget()
        endImage = None
    
    encryptImgFrame = tk.Frame(imageFrame, height=450, width = 300, relief = 'ridge')
    encryptImgFrame.pack(side = 'left', padx=25, pady=25)

    newImgFrame = tk.Frame(imageFrame, height=450, width= 300, relief='ridge')
    newImgFrame.pack(side='right', padx=25, pady=25)
    
    #resize image to fit in display viewer
    image = Image.open(inFile)
    basewidth = 300
    wpercent = (basewidth/float(image.size[0]))
    hsize = int((float(image.size[1])*float(wpercent)))
    image = image.resize((basewidth,hsize), Image.ANTIALIAS)

    startPhoto=ImageTk.PhotoImage(image)

    startImage = tk.Label(encryptImgFrame, image=startPhoto, height = 450, width = 300, text= 'Original Image')
    startImage.image = startPhoto
    startImage.pack()

def encryptFn(event):
    global inFile
    global outFile
    global newImgFrame
    global endImage
    global inputBox

        
    outFile = filedialog.asksaveasfilename(defaultextension = "png",initialdir= '/',title = 'Save As...', filetypes = (("PNG files","*.png"),("all files","*.*")))
    
    if outFile:
        x = DCT()
        secretMess = inputBox.get()
        inputBox.delete(0,'end')
        secret = x.DCTEn(inFile, secretMess, outFile)

        newImgFrame.pack_forget()
        if endImage:
            endImage.pack_forget()

        #resize image to fit in display viewer
        image = Image.open(outFile)
        newImgFrame = tk.Frame(imageFrame, height=450, width = 300, relief = 'ridge')
        newImgFrame.pack(side = 'right', padx=25, pady=25)
        
        basewidth = 300
        wpercent = (basewidth/float(image.size[0]))
        hsize = int((float(image.size[1])*float(wpercent)))
        image = image.resize((basewidth,hsize), Image.ANTIALIAS)

        photo=ImageTk.PhotoImage(image)

        endImage = tk.Label(newImgFrame, image=photo, height = 450, width = 300, text = 'Encrypted Image')
        endImage.image = photo
        endImage.pack()


    else:
        print('No filename given')
        return

def encryptControl(event):
    encryptWindow()

    global inFile 
    inFile = None

#DECRYPT FUNCTIONS
def goBackDe(event):
    global decryptScreen

    decryptScreen.pack_forget()

    openingScreen()

def decryptFn(event):
    global deFile
    global outBox
    global messageFrame

    if deFile:
        y = DCT()
        decode = y.DCTDe(deFile)
        outBox.config(state = 'normal')
        outBox.delete(0,'end')
        outBox.insert(0,decode)
        outBox.config(state = 'disabled')
    else:
        print('No filename given')
        return

def decryptWindow():
    global startScreen
    global decryptScreen
    global decryptImgFrame
    global imageFrame
    global buttonFrame
    global messageFrame
    global outBox

    root.title("Decrypt -- Image Hider v2.2.3")
    root.geometry('{}x{}'.format(750,650))
    startScreen.pack_forget()

    decryptScreen = tk.Frame(root, height =600, width = 750)
    decryptScreen.pack(expand = False)

    #make frames for encryption screen
    imageFrame = tk.Frame(decryptScreen, height = 500, width = 750,bg='blue')
    imageFrame.pack(fill='x', expand = True)

    messageFrame = tk.Frame(decryptScreen, height = 50, width = 750, bg='black')
    messageFrame.pack(fill='x', expand = True)

    buttonFrame = tk.Frame(decryptScreen, height = 50,width = 750)
    buttonFrame.pack(fill ='both', expand = True)

    ### making all buttons
    fileChooseButton = tk.Button(buttonFrame, text="Choose File...", command=lambda:chooseFileFnD(None))
    fileChooseButton.pack(side='left', padx = 77)

    eButton = tk.Button(buttonFrame, text="Decrypt", command=lambda:decryptFn(None))
    eButton.pack(side='left', padx = 77)

    backButton = tk.Button(buttonFrame, text="Back", command=lambda:goBackDe(None))
    backButton.pack(side='left', padx=77)

    ##make image windows
    decryptImgFrame = tk.Frame(imageFrame, height=450, width = 600, relief = 'ridge')
    decryptImgFrame.pack( padx=25, pady=25)

    #make encryption message input box
    outBox = tk.Entry(messageFrame, relief = 'ridge', width = 250, state = 'disabled', fg = 'black')
    outBox.pack(padx = 25, pady =25)

def decryptControl(event):
    decryptWindow()

    global deFile 
    deFile = None

def chooseFileFnD(event):
    global deFile
    global decryptScreen
    global decryptImgFrame
    global startImage


    deFile = openFunc(None)

    if deFile == None:
        print("Decryption Error: File Not Opened!")
        return

    #convert jpeg to png if needed
    if deFile[-4:]==".jpg":
        print("Decrytion Error: File format not supported")
        return

    #clean image displays
    decryptImgFrame.pack_forget()

    decryptImgFrame = tk.Frame(imageFrame, height=450, width = 600, relief = 'ridge')
    decryptImgFrame.pack(padx=25, pady=25)

    #resize image to fit in display viewer
    image = Image.open(deFile)
    basewidth = 600
    wpercent = (basewidth/float(image.size[0]))
    hsize = int((float(image.size[1])*float(wpercent)))
    image = image.resize((basewidth,hsize), Image.ANTIALIAS)

    startPhoto=ImageTk.PhotoImage(image)

    startImage = tk.Label(decryptImgFrame, image=startPhoto, height = 450, width = 600)
    startImage.image = startPhoto
    startImage.pack()

#OPENING FUNCTIONS

def center(win):
    """
    centers a tkinter window
    :param win: the root or Toplevel window to center
    """
    win.update_idletasks()
    width = win.winfo_width()
    frm_width = win.winfo_rootx() - win.winfo_x()
    win_width = width + 2 * frm_width
    height = win.winfo_height()
    titlebar_height = win.winfo_rooty() - win.winfo_y()
    win_height = height + titlebar_height + frm_width
    x = win.winfo_screenwidth() // 2 - win_width // 2 -250
    y = win.winfo_screenheight() // 2 - win_height // 2 -200
    win.geometry('{}x{}+{}+{}'.format(width, height, x, y))
    win.deiconify()

def openingScreen():
    #make start window
    global startScreen
    
    root.geometry('{}x{}'.format(250,250))
    startScreen = tk.Frame(root, height = 250, width = 250)
    startScreen.pack(expand=True)

    encryptButton = tk.Button(startScreen, text="Encrypt", command=lambda:encryptControl(None))
    encryptButton.pack()
    decryptButton = tk.Button(startScreen, text="Decrypt", command=lambda:decryptControl(None))
    decryptButton.pack()

    root.title("Image Hider v2.2.3")
    center(root)

def main():
    global quit
    quit = 0

    openingScreen()
    
    root.protocol("WM_DELETE_WINDOW", lambda:exitFunc(None))

    while quit ==0:
        root.update_idletasks()
        root.update()


if __name__ == '__main__':
    main()