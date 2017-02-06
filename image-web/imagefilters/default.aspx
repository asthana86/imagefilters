<%@ Page Title="" Language="C#" MasterPageFile="~/MasterPage.Master" AutoEventWireup="true" CodeBehind="default.aspx.cs" Inherits="crimecopcpp._default" %>

<asp:Content ID="Content1" ContentPlaceHolderID="head" runat="server">
    <link rel="stylesheet" href="css/font-awesome.css" type="text/css" />
</asp:Content>
<asp:Content ID="Content2" ContentPlaceHolderID="maincontent1" runat="server">
    <style>
        body {
            background-color: ghostwhite;
        }

        h1 {
            color: maroon;
            margin-left: 10px;
            margin: 60px 0 0 40px;
        }

        .iconDetails {
            margin-left: 2%;
            float: left;
            height: 88px;
            width: 81px;
        }

        .box {
            width: 700px;
            height: auto;
            padding: 1%;
            float: left;
        }

        .left {
            float: left;
            width: 49px;
            height: 86px;
        }

        .right {
            float: left;
            margin: 0px 0 0 5px;
            width: 500px;
        }

        #logo {
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
        }
        #content {
            width: 100%;
            height: 100px;
            position: absolute;
            bottom: 600px;
            margin: 0 0 0 15px;
        }
        #imagecontainer {
            position: fixed;
            top: 0;
            left: 0;
            width: 100%;
            height: 100%;
        }
    </style>
    <div id='logo' class='box'>
        <div class="left">
            <img src='images/resize.png' class='iconDetails'/>
        </div>
        <div class="right">
            <h1>Image Filters using OpenCV</h1>
        </div>
        <div id="content" class="box">
            <p>Upload the image to resize.</p>
            <asp:FileUpload ID="ImgUpload" runat="server" />
            <asp:Button ID="BtnUpload" runat="server" Text="Upload" OnClick="BtnUpload_Click" />
            <asp:Label ID="lblstatus" runat="server"></asp:Label>
            <asp:Label ID="lbloriginalsize" runat="server"></asp:Label>
            <asp:Button ID="BtnSrink" runat="server" Text="Grayscale" OnClick="BtnSrink_Click" />
            <asp:Label ID="lblshrinksize" runat="server"></asp:Label>
        </div>
    </div>
</asp:Content>
