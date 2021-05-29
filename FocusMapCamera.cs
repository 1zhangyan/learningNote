using MoreMountains.Tools;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

namespace MoreMountains.TopDownEngine
{
public class FocusMapCamera : MonoBehaviour {
    public Camera fucoscamera;
    public Transform fucosplayerIcon;
    public  GameObject fucosmap;
    public  GameObject fucosObject;
    
    // Use this for initialization
    // protected Character player;

    void Awake()
    {
       fucosmap.SetActive(false);
    }

    void Start () {
       
	}

    public void SetFocusObj(GameObject inputObj)
    {
        fucosObject = inputObj;
    }
	public void ShowFucosMap()
    {
            if (fucosmap!= null)
                {
                    fucoscamera.transform.position = new Vector3(fucosObject.transform.position.x, fucoscamera.transform.position.y, fucosObject.transform.position.z); 
                    fucosmap.SetActive(true);
                }
    }
 
	// Update is called once per frame
	void Update () {
        ShowFucosMap();
    }
}
}
