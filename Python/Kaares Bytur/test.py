width = 100
bg_dict = {
        "1" : [0,width],
        "2" : [0,width],
        "3" : [0,width]
    }

print(bg_dict["1"][1])

def parallax_bg():
    bg_dict = {
        "1" : [0,width],
        "2" : [0,width],
        "3" : [0,width]
    }
    for key in parallax_bg:
        screen.blit(bg_images[int(key)+1],(parallax_bg[key][0],0))
        screen.blit(bg_images[int(key)+1],(parallax_bg[key][1],0))
        
        parallax_bg[key][0] += int(key)
        parallax_bg[key][1] += int(key)

        if parallax_bg[key][0] < -width:
            parallax_bg[key][0] = width
        if parallax_bg[key][1] < -width:
            parallax_bg[key][1] = width
