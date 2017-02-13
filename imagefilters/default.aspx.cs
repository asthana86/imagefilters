using System;
using System.Collections.Generic;
using System.Linq;
using System.Web;
using System.Web.UI;
using System.Web.UI.WebControls;
using System.IO;
using Microsoft.Azure;
using Microsoft.WindowsAzure.Storage;
using Microsoft.WindowsAzure.Storage.Blob;
using System.Web.UI.HtmlControls;
using System.Configuration;
using Microsoft.WindowsAzure.Storage.Queue;
using System.Text;

namespace crimecopcpp
{
    public partial class _default : System.Web.UI.Page
    {
        protected void uploadimagetoblob(FileUpload imgUpload)
        {
            byte[] bytes1 = new byte[100];
            Random rnd1 = new Random();
            int number = rnd1.Next(10000);
            CloudStorageAccount storageAccount = CloudStorageAccount.Parse(CloudConfigurationManager.GetSetting("cppstorage_AzureStorageConnectionString"));
            CloudBlobClient blobClient = storageAccount.CreateCloudBlobClient();
            CloudBlobContainer container = blobClient.GetContainerReference("img-container");
            container.CreateIfNotExists();
            CloudBlockBlob blob = container.GetBlockBlobReference("img-blob" + number);
            blob.UploadFromStream(imgUpload.PostedFile.InputStream);
            CloudQueueClient queueClient = storageAccount.CreateCloudQueueClient();
            CloudQueue queue = queueClient.GetQueueReference("img-input-queue");
            queue.CreateIfNotExists();

            string valstr = "img-blob" + number;
            byte[] value = new byte[valstr.Length];
            for (int i = 0; i < valstr.Length; i++)
                value[i] = Convert.ToByte(valstr[i]);
        
            CloudQueueMessage message = new CloudQueueMessage(value);
            queue.AddMessage(message);
        }

        protected void Page_Load(object sender, EventArgs e)
        {
            if (string.IsNullOrEmpty(Page.Title))
            {
                Page.Title = ConfigurationManager.AppSettings["Image Filters"];  //title saved in web.config
            }
        }

        protected void BtnUpload_Click(object sender, EventArgs e)
        {
            if (ImgUpload.HasFile)
            {
                if ((ImgUpload.PostedFile.ContentType == "image/png") ||
                    (ImgUpload.PostedFile.ContentType == "image/jpeg") ||
                    (ImgUpload.PostedFile.ContentType == "image/gif") ||
                    (ImgUpload.PostedFile.ContentType == "image/bmp"))
                {
                    string photoFolder = Path.Combine(@"C:\image\crimecop\crimecopcpp\crimecopcpp\images", User.Identity.Name);
                    if (Convert.ToInt64(ImgUpload.PostedFile.ContentLength) < 10000000)
                    {
                        if (Directory.Exists(photoFolder))
                        {
                            Directory.CreateDirectory(photoFolder);
                        }
                        ImgUpload.SaveAs(photoFolder + "\\" + ImgUpload.FileName);


                        uploadimagetoblob(ImgUpload);
                        lblstatus.Text = "<font color='Green'> Sucessfully uploaded " + ImgUpload.FileName;

                        long size = Convert.ToInt64(ImgUpload.PostedFile.ContentLength);

                        lbloriginalsize.Text = "<font color='Red'> Original Image size " + size;


                        /*Image img = new Image();
                       img.ImageUrl = "~/" + filename;
                       this.Controls.Add(img);*/

                    }
                    else
                    {
                        lblstatus.Text = "File must be less than 10 MB.";
                    }
                }
                else
                {
                    lblstatus.Text = "File must be of type jpeg, png, gif or bmp.";
                }
            }
            else
            {
                lblstatus.Text = "No file selected";
            }



        }

        protected void BtnSrink_Click(object sender, EventArgs e)
        {
            CloudStorageAccount storageAccount = CloudStorageAccount.Parse(CloudConfigurationManager.GetSetting("cppstorage_AzureStorageConnectionString"));
            CloudBlobClient blobClient = storageAccount.CreateCloudBlobClient();
            CloudBlobContainer container = blobClient.GetContainerReference("img-resized-container");
            container.CreateIfNotExists(); 

            foreach (IListBlobItem item in container.ListBlobs(null, false))
            {
                if (item.GetType() == typeof(CloudBlockBlob))
                {
                    CloudBlockBlob blobitem = (CloudBlockBlob)item;

                    Console.WriteLine("Block blob of length {0}: {1}", blobitem.Properties.Length, blobitem.Uri);

                    CloudBlockBlob blob = container.GetBlockBlobReference(blobitem.Name);
                    using (var fileStream = System.IO.File.OpenWrite(@"D:\image\imagefilters\imagefilters\imagefilters\images\" + blobitem.Name + ".jpg"))
                    {
                        blob.DownloadToStream(fileStream);
                    }

                    FileInfo fi = new FileInfo(@"D:\image\imagefilters\imagefilters\imagefilters\images\" + blobitem.Name + ".jpg");
                    long fileSizeInBytes = fi.Length;

                    lblshrinksize.Text = "<font color='Red'> Grayscale Image size " + fileSizeInBytes;

                    for (int i = 0; i < 12; i++)
                    {
                        this.Controls.Add(new LiteralControl("<br />"));
                    }

                    Image image = new Image();
                    image.ImageUrl = "~/images/" + blobitem.Name + ".jpg";
                    this.Controls.Add(image);
                    blobitem.Delete();
                }
                else if (item.GetType() == typeof(CloudPageBlob))
                {
                    Console.WriteLine("No Block Blob available");
                }
            }
        }
    }
}