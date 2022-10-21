using System.Collections;
using UnityEngine;

public class TimeDelayedKill : MonoBehaviour
{
    /**
     * @brief The time to wait before killing the object
     */
    public float timeToWait;
    
    // Start is called before the first frame update
    void Start()
    {
        StartCoroutine(Kill());
    }
    
    /**
     * @brief Kills the object after a certain time
     */
    IEnumerator Kill()
    {
        yield return new WaitForSeconds(timeToWait);
        Destroy(gameObject);
    }
}