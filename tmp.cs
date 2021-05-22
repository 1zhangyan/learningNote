using UnityEngine;
using System.Collections;
using MoreMountains.Tools;

namespace MoreMountains.TopDownEngine
{
    /// <summary>
    /// Add this class to a GameObject to have it play a background music when instanciated.
    /// Careful : only one background music will be played at a time.
    /// </summary>
    [AddComponentMenu("TopDown Engine/Sound/BackgroundMusic")]
    public class BackgroundMusic : MMSingleton<BackgroundMusic> 
    {
        /// the background music
        [Tooltip("the audio clip to use as background music")]
        public AudioClip SoundClip0;
        public AudioClip SoundClip1;
        public AudioClip SoundClip2;

        protected AudioSource _source;

        /// <summary>
        /// Gets the AudioSource associated to that GameObject, and asks the GameManager to play it.
        /// </summary>

        public void SwapBGM(int teleportId)
        {
            if(teleportId == 0||teleportId == 3)
                BindAndPlay(SoundClip1);
            if(teleportId == 1)
                BindAndPlay(SoundClip0);
            if(teleportId == 2)
                BindAndPlay(SoundClip2);

        }
        protected virtual void BindAndPlay(AudioClip SoundClip)
        {
            
            _source.clip = SoundClip;
            SoundManager.Instance.PlayBackgroundMusic(_source);
        }

        protected virtual void Start()
        {
            _source = gameObject.AddComponent<AudioSource>() as AudioSource;
            _source.playOnAwake = false;
            _source.spatialBlend = 0;
            _source.rolloffMode = AudioRolloffMode.Logarithmic;
            _source.loop = true;
            BindAndPlay(SoundClip1);
        }

    }
}