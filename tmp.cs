using MoreMountains.Tools;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace MoreMountains.TopDownEngine
{
public class miniMapCamera : MonoBehaviour {
    public Camera minicamera;
    public Transform miniplayerIcon;//小地图人物图标
    public Transform maxplayerIcon;
    public  GameObject maxmap;//大地图
    public GameObject minimap;//小地图
    public int mapswapflag;
    // Use this for initialization
    // protected Character player;

    void Awake()
    {
       
    }

    void Start () {
        OpenMinimap();
	}

	public void OpenMaxmap()
    {
        maxmap.gameObject.SetActive(true);
        minimap.gameObject.SetActive(false);
        mapswapflag = 1;
    }
 
    public void OpenMinimap()
    {
        maxmap.gameObject.SetActive(false);
        minimap.gameObject.SetActive(true);
        mapswapflag = 0;
    }
    
    public void SwapMap(Character player)
    {
        if(mapswapflag == 1)
        {
            minicamera.transform.position = new Vector3(player.transform.position.x, (minicamera.transform.position.y)/3*2, player.transform.position.z);
            OpenMinimap();
            //miniplayerIcon.eulerAngles = new Vector3(0, 0, -player.transform.eulerAngles.y);
        }
        else
        {
            minicamera.transform.position = new Vector3(player.transform.position.x, (minicamera.transform.position.y)*3/2, player.transform.position.z);
            OpenMaxmap();
            //maxplayerIcon.eulerAngles = new Vector3(0, 0, -player.transform.eulerAngles.y);
        }
    }

	// Update is called once per frame
	void Update () {
        Character player = LevelManager.Instance.Players[0];
        minicamera.transform.position = new Vector3(player.transform.position.x, minicamera.transform.position.y, player.transform.position.z);
        if(mapswapflag == 0)
            miniplayerIcon.eulerAngles = new Vector3(0, 0, -player.transform.eulerAngles.y);
        else 
            maxplayerIcon.eulerAngles = new Vector3(0, 0, -player.transform.eulerAngles.y);
        if (Input.GetKeyDown(KeyCode.M))
        {
           SwapMap(player);
        }
    }
}
}
