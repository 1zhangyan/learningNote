using UnityEngine;
using System.Collections;
using MoreMountains.Tools;
using UnityEngine.UI;
using System;

namespace MoreMountains.TopDownEngine
{
    public class UITextManager : MMSingleton<UITextManager> 
    {

        public int dissapearTime = 3;
        public Text obj;
        
        public string text;
    
        public float dt = 0.007f;//打字间隔时间
        public float showingTime = 1f;//显示使用的时间

        private string ColorToHex(Color color) //十进制转十六进制
        {
            int r = Mathf.RoundToInt(color.r * 255.0f);
            int g = Mathf.RoundToInt(color.g * 255.0f);
            int b = Mathf.RoundToInt(color.b * 255.0f);
            int a = Mathf.RoundToInt(color.a * 255.0f);
            string hex = string.Format("{0:X2}{1:X2}{2:X2}", r, g, b);
            return hex;
        }
        private IEnumerator Typing(string text)
        {
            float Startime = Time.time, aTime, dTime = Time.time;
            //-----
            int index = 0;
            float timeScale;
            int a = 0;
            bool start = false;
            //-----
            if (obj)
            {
                while (index < text.Length || a < 255)
                {
                    obj.text = "";
                    timeScale = 256 / (index * showingTime);
                    aTime = (Time.time - Startime) * timeScale;
                    for (int i = 0; i <= index && i < text.Length; i++)
                    {
                        a = (int)(aTime * (index - i));
                        a = Mathf.Clamp(a, 0, 255);
    
                        if (a == 255 && i == 0 && start == false)
                        {
                            obj.text += "<color=#" + ColorToHex(obj.color) + "ff>";
                            start = true;
                        }
                        if (a == 255 && start)
                        {
                            obj.text += text[i];
                            continue;
                        }
                        if (a != 255 && start)
                        {
                            start = false;
                            obj.text += "</color>";
                        }
    
                        string aStr = Convert.ToString(a, 16);
                        aStr = (aStr.Length == 1 ? "0" : "") + aStr;
                        obj.text += "<color=#" + ColorToHex(obj.color) + aStr + ">" + text[i] + "</color>";
                    }
                    if (a == 255 && start)
                        obj.text += "</color>";
                    if (Time.time - dTime >= dt)
                    {
                        dTime = Time.time;
                        index++;
                    }
                    yield return 0;
                }
            }
        }

        public void Play(string text)
        {
            StartCoroutine(Typing(text));
        }    

        public void Setinvalid()
        {
            obj.gameObject.SetActive(false);
        }
        public void Update()
        {
            if (Input.GetKeyDown(KeyCode.N))
            {
                obj.gameObject.SetActive(true);
                Play("This is testText");
                Invoke("Setinvalid" , dissapearTime);
            }   
        }
    }
}