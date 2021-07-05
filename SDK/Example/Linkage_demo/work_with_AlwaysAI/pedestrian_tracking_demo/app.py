import edgeiq
import cv2
import math

def main():
    fps = edgeiq.FPS()

    # Change parameter to alwaysai/human_pose_eyecloud to run the human pose model.
    with edgeiq.EyeCloud('alwaysai/mobilenet_ssd_eyecloud'
                         ) as camera, edgeiq.Streamer() as streamer:
                         

        fps.start()

        centroid_tracker = edgeiq.CentroidTracker(deregister_frames=20, max_distance=100)

        number=0

        while True:

            text = ["FPS:{}".format(fps.compute_fps())]

            frame = camera.get_frame()

            #print('image sequence = {}'.format(frame.sequence_index))

            result = camera.get_model_result(confidence_level=0.9)

        

            # Check for inferencing results.
            if result:
                #print('model sequence = {}'.format(result.sequence_index))

                text.append("Model: {}".format(camera.model_id))

                if camera.model_purpose == 'PoseEstimation':
                    frame = result.draw_poses(frame)

                    text.append("Inference time: {:1.3f} s".format(result.duration))

                    for ind, pose in enumerate(result.poses):
                        text.append("Person {}".format(ind))
                        text.append('-' * 10)
                        text.append("Key Points:")
                        for key_point in pose.key_points:
                            text.append(str(key_point))

                elif camera.model_purpose == 'ObjectDetection':
                  

                    #移除交并比过大的检测框
                    if(len(result.predictions)>1):
                        for i,prediction in enumerate(result.predictions):
                            for j in range((len(result.predictions)-1),i,-1):
                                #print("i：%s  j：%s"%(i,j))
                                IOU=prediction.box.compute_overlap(result.predictions[j].box)
                                print(IOU)
                                if(IOU>0.3):
                                    #print("距离过近")
                                    result.predictions.pop()
                    

                    #划定FOV有效检测区
                    for i,prediction in  enumerate(result.predictions):
                        if(prediction.box.center[0]<500 or prediction.box.center[0]>1300):
                            result.predictions.pop(i)

                        #print("x：%s  y：%s"%(prediction.box.center[0],prediction.box.center[1]))

                    objects = centroid_tracker.update(result.predictions)

                    text.append("Inference time: {:1.3f} s".format(result.duration))
                    text.append("Objects:")

                    
               
                    for (object_id, prediction) in objects.items():
                        new_label = 'person {}'.format(object_id)
                        
                        if(object_id+1>number):
                            number=object_id+1

                        prediction.label = new_label
                        text.append("{}: {:2.2f}%".format(prediction.label, prediction.confidence * 100))
                        result.predictions.append(prediction)          
                    
                    
                    frame = edgeiq.markup_image(frame, result.predictions)

                    text.append("people pass: {}".format(number))





                elif camera.model_purpose == 'Classification':
                    if len(result.predictions) > 0:
                        top_prediction = result.predictions[0]
                        text = "Classification: {}, {:.2f}%".format(
                            top_prediction.label,
                            top_prediction.confidence * 100)
                    else:
                        text = None

                    cv2.putText(frame, text, (5, 25), cv2.FONT_HERSHEY_SIMPLEX,
                                0.4, (0, 0, 255), 2)
            
            cv2.rectangle(frame,(500,0),(1300,1080),(255,0,0), 2)

            streamer.send_data(frame, text)

            if streamer.check_exit():
                break

            fps.update()

        print('fps = {}'.format(fps.compute_fps()))


if __name__ == '__main__':
    main()
