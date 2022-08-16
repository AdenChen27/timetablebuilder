VERSION 5.00
Begin VB.Form main 
   BackColor       =   &H00FFFFFF&
   BorderStyle     =   1  'Fixed Single
   Caption         =   "Zion�ſ�����"
   ClientHeight    =   4410
   ClientLeft      =   45
   ClientTop       =   390
   ClientWidth     =   8625
   BeginProperty Font 
      Name            =   "΢���ź�"
      Size            =   9
      Charset         =   134
      Weight          =   400
      Underline       =   0   'False
      Italic          =   0   'False
      Strikethrough   =   0   'False
   EndProperty
   LinkTopic       =   "Form1"
   MaxButton       =   0   'False
   MinButton       =   0   'False
   ScaleHeight     =   4410
   ScaleWidth      =   8625
   StartUpPosition =   3  '����ȱʡ
   Begin VB.TextBox time_table_filename 
      Height          =   495
      Left            =   4410
      TabIndex        =   7
      Top             =   2400
      Width           =   3795
   End
   Begin VB.TextBox course_num_per_day 
      Height          =   495
      Left            =   3200
      TabIndex        =   6
      Text            =   "8"
      Top             =   1680
      Width           =   5000
   End
   Begin VB.TextBox course_filename 
      Height          =   495
      Left            =   3200
      TabIndex        =   4
      Text            =   "�γ���Ϣ.xls"
      Top             =   960
      Width           =   5000
   End
   Begin VB.TextBox teacher_filename 
      Height          =   495
      Left            =   3200
      TabIndex        =   3
      Text            =   "��ʦ��Ϣ.xls"
      Top             =   240
      Width           =   5000
   End
   Begin VB.CommandButton generatetime_table 
      BackColor       =   &H00FFFFFF&
      Caption         =   "���ɿγ̱�"
      BeginProperty Font 
         Name            =   "΢���ź�"
         Size            =   20.25
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   705
      Left            =   1200
      MaskColor       =   &H00FFFFFF&
      Style           =   1  'Graphical
      TabIndex        =   2
      Top             =   3360
      Width           =   5895
   End
   Begin VB.Label Label1 
      BackColor       =   &H00FFFFFF&
      Caption         =   "���ɿγ̱��ŵ�ַ:"
      BeginProperty Font 
         Name            =   "����"
         Size            =   20.25
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   120
      TabIndex        =   8
      Top             =   2400
      Width           =   4440
   End
   Begin VB.Label course_num_per_day_label 
      BackColor       =   &H00FFFFFF&
      Caption         =   "ÿ��γ�����:"
      BeginProperty Font 
         Name            =   "����"
         Size            =   20.25
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   105
      TabIndex        =   5
      Top             =   1680
      Width           =   3000
   End
   Begin VB.Label course_filename_label 
      BackColor       =   &H00FFFFFF&
      Caption         =   "�γ���Ϣ��ַ:"
      BeginProperty Font 
         Name            =   "����"
         Size            =   20.25
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   120
      TabIndex        =   1
      Top             =   960
      Width           =   3000
   End
   Begin VB.Label teacher_filename_label 
      BackColor       =   &H00FFFFFF&
      Caption         =   "��ʦ��Ϣ��ַ:"
      BeginProperty Font 
         Name            =   "����"
         Size            =   20.25
         Charset         =   134
         Weight          =   400
         Underline       =   0   'False
         Italic          =   0   'False
         Strikethrough   =   0   'False
      EndProperty
      Height          =   495
      Left            =   100
      TabIndex        =   0
      Top             =   240
      Width           =   3000
   End
End
Attribute VB_Name = "main"
Attribute VB_GlobalNameSpace = False
Attribute VB_Creatable = False
Attribute VB_PredeclaredId = True
Attribute VB_Exposed = False

Private Sub course_num_per_day_KeyPress(KeyAscii As Integer)
    If KeyAscii = 8 Then
        Exit Sub
    End If
    If KeyAscii < 48 Or KeyAscii > 57 Then
        KeyAscii = 0
    End If
End Sub


Private Sub Form_Load()
    If Dir("generate_time_table.exe") = "" Then
        a = MsgBox("ȱ��generate_time_table.exe", vbOKOnly + vbExclamation, "����")
        End
    End If
    If Dir("input.exe") = "" Then
        a = MsgBox("ȱ��input.exe", vbOKOnly + vbExclamation, "����")
        End
    End If
    If Dir("output.exe") = "" Then
        a = MsgBox("ȱ��output.exe", vbOKOnly + vbExclamation, "����")
        End
    End If
    time_table_filename.Text() = App.Path + "\" + "�γ̱�.xls"
End Sub

Private Sub generatetime_table_Click()
    ' check teacher info filename is available
    If Dir(App.Path + "\" + teacher_filename.Text()) = "" Then
        a = MsgBox("��ʦ��Ϣ��ַ����ȷ", vbOKOnly + vbExclamation, "����")
        Exit Sub
    End If
    If teacher_filename.Text() = "" Then
        a = MsgBox("��ʦ��Ϣ��ַ����ȷ", vbOKOnly + vbExclamation, "����")
        Exit Sub
    End If
   
    If Dir(App.Path + "\" + course_filename.Text()) = "" Then
        a = MsgBox("�γ���Ϣ��ַ����ȷ", vbOKOnly + vbExclamation, "����")
        Exit Sub
    End If
    If course_filename.Text() = "" Then
        a = MsgBox("�γ���Ϣ��ַ����ȷ", vbOKOnly + vbExclamation, "����")
        Exit Sub
    End If
    If course_num_per_day.Text() = "" Then
        a = MsgBox("ÿ��γ���������ȷ", vbOKOnly + vbExclamation, "����")
        Exit Sub
    End If
    
    Dim command As String
    command = "generate_time_table.exe """
    command = command & teacher_filename.Text() & """ """
    command = command & course_filename.Text() & """ """
    command = command & CInt(course_num_per_day.Text()) & """ """
    command = command & time_table_filename.Text() & """"
    Shell command, 1
End Sub
