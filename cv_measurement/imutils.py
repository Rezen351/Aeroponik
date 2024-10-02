import cv2
import numpy as np
import matplotlib.pyplot as plt
import os
import random

# Setting Variables
DIMX=(700,1400)
PPM=6.0

def show_img(name,img):
    cv2.imshow(name,img)
    cv2.waitKey(0)
    cv2.destroyAllWindows()

def crop_img(img,dimx=DIMX,debug=False):
    # crop image:
    crop_img = img[:,dimx[0]:dimx[1]]
    # plt.imshow(crop_img)
    if debug: show_img('cropped',crop_img)
    return crop_img

def apply_clahe(img,clipLim=2.0,gridSize=(8,8),debug=False):
    # apply CLAHE to image:
    clahe = cv2.createCLAHE(clipLimit=clipLim, tileGridSize=gridSize)

    # convert the cropped image to grayscale
    gray_img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    cl = clahe.apply(gray_img)
    # plt.imshow(cl, cmap=plt.get_cmap('gray'))

    # cl = cv2.GaussianBlur(cl, (kernel_size, kernel_size), 0)
    if debug: cv2.imshow('clahe',cl)
    # if debug: show_img('clahe',cl)
    return cl

def sharpening(img,kernel_size=3,debug=False):
    # Apply high-boost filtering
    blurred = cv2.GaussianBlur(img, (kernel_size, kernel_size), 0)
    sharped = cv2.addWeighted(img, 1.5, img-blurred, 0.5, 0)
    # laplacian = cv2.Laplacian(crop_img, cv2.CV_64F)
    # sharpened = cv2.add(crop_img, laplacian)
    if debug: cv2.imshow('sharp',sharped)
    return sharped

def adaptive_threshold(img,blockSize=181,C=1,debug=False):
    # convert the image to grayscale if it's not already
    if len(img.shape) > 2:
        img = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)
    # ensure the image data type is uint8
    img = img.astype('uint8')
    # apply adaptive thresholding
    th = cv2.adaptiveThreshold(img, 255, cv2.ADAPTIVE_THRESH_MEAN_C, cv2.THRESH_BINARY, blockSize=blockSize, C=C)
    # th = cv2.adaptiveThreshold(img, 255, cv2.ADAPTIVE_THRESH_GAUSSIAN_C, cv2.THRESH_BINARY, blockSize=blockSize, C=C)
    # plt.imshow(th, cmap=plt.get_cmap('gray'))
    if debug: cv2.imshow('threshold',th)
    # if debug: show_img('threshold',th)
    return th

def morph(img,type,size=(5,5),debug=False):
    kernel = np.ones(size,np.uint8)
    morph = cv2.morphologyEx(img, type, kernel)
    # plt.imshow(morph, cmap=plt.get_cmap('gray'))
    # if debug: show_img('morph',morph)
    if debug: cv2.imshow('morph',morph)
    return morph

def show_segment(img,th,ppm=PPM,alpha=0.5,lim_cm=20.0,threshold_value=160,debug=False):
    status = None
    colors=[(0,0,255),(0,255,0)]
    lim = np.floor(lim_cm*ppm*10)

    contours, hierarchy = cv2.findContours(th, cv2.RETR_TREE, cv2.CHAIN_APPROX_SIMPLE)
    contours = sorted(contours, key=cv2.contourArea, reverse=True)
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # # Get biggest segment:
    # sel_cnt = max(contours, key=cv2.contourArea)

    # Contour based on the brightest image
    # Sort contours by area in descending order
    filtered_contours = []
    for cnt in contours:
        mask = np.zeros_like(gray)
        cv2.drawContours(mask, [cnt], -1, 255, -1)
        masked_gray = cv2.bitwise_and(gray, gray, mask=mask)
        # average_pixel_value = np.max(masked_gray[mask == 255])
        average_pixel_value = np.mean(masked_gray[mask == 255])
        if average_pixel_value > threshold_value:
            filtered_contours.append(cnt)
    sel_cnt = max(filtered_contours, key=cv2.contourArea)

    # pixel_averages = []
    # for cnt in contours[:10]:
    # # for cnt in n_largest_contours:
    #     mask = np.zeros_like(gray)
    #     cv2.drawContours(mask, [cnt], -1, 255, -1)
    #     masked_gray = cv2.bitwise_and(gray, gray, mask=mask)
    #     # average_pixel_value = np.sum(masked_gray[mask == 255])
    #     average_pixel_value = np.mean(masked_gray[mask == 255])
    #     pixel_averages.append(average_pixel_value)
    # highest_average_index = np.argmax(pixel_averages)
    # sel_cnt = contours[highest_average_index]

    x, y, w, h = cv2.boundingRect(sel_cnt)
    h_cm = h/ppm/10.0

    if h > lim:
        color = colors[1]
        status = True
    else:
        color = colors[0]
        status = False

    # Create a mask for the largest contour
    mask = img.copy()
    cv2.drawContours(mask, [sel_cnt], -1, color, -1)
    result = cv2.addWeighted(img,alpha,mask,1-alpha,0)

    # draw bounding box:
    rect_img = result.copy()
    cv2.rectangle(rect_img, (x, y), (x + w, y + h), color, 2)

    # # Draw the horizontal line
    # cv2.line(rect_img, (0, lim), (img.shape[1], lim), (255,0,0), 3)

    # Draw length text
    # Get text dimensions
    text = f'h:{h_cm:.2f}cm'
    font = cv2.FONT_HERSHEY_SIMPLEX
    fontScale = 1
    thickness = 2
    text_width, text_height = cv2.getTextSize(text, font, fontScale, thickness)[0]

    # cv2.putText(rect_img, f'h:{h_cm:.2f}cm', (x+w+20, y+h//2), cv2.FONT_HERSHEY_SIMPLEX, 1, (255,0,0), 2, cv2.LINE_AA)
    cv2.rectangle(rect_img, (x+2, y+2), (x+text_width+10, y+text_height+10), color, -1)
    cv2.putText(rect_img, text, (x+5,y+text_height+5), font, fontScale, (255,0,0), thickness, cv2.LINE_AA)

    # show image:
    # plt.imshow(rect_img)
    if debug:
        show_img('segment',rect_img)
        print(x,y,w,h)
    return rect_img,status,h_cm

# def get_roi(img,cropped,dimx=DIMX):
#     img_roi = img.copy()
#     img_roi[:,dimx[0]:dimx[1]]=cropped
#     return img_roi

def get_roi(img, cropped, dimx=DIMX):
    img_roi = img.copy()
    h, w, _ = cropped.shape
    img_roi[:, dimx[0]:dimx[0]+w] = cropped
    return img_roi

# Image Processing
def check_length(img,dimx,ppm,lim_cm,debug=False):
#   img = cv2.imread(im_path)
  crop_im = crop_img(img,dimx)
  cl = apply_clahe(crop_im)
  sharp = sharpening(cl)
  th = adaptive_threshold(sharp)
  im_open = morph(th,cv2.MORPH_OPEN)
  final_im,status,length = show_segment(crop_im,im_open,ppm=ppm,lim_cm=lim_cm)
  full_im=get_roi(img,final_im)
  if debug:
    deb_im = np.concatenate((crop_im, cv2.cvtColor(cl, cv2.COLOR_GRAY2BGR), cv2.cvtColor(sharp, cv2.COLOR_GRAY2BGR), cv2.cvtColor(th, cv2.COLOR_GRAY2BGR), cv2.cvtColor(im_open, cv2.COLOR_GRAY2BGR), final_im), axis=1)
    cv2.imshow('Debug Image', deb_im)
    cv2.imshow('Full Image', full_im)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
    return full_im,deb_im,status,length
  return full_im,None,status,length

  
