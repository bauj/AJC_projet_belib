    /* Couleurs dans la figure*/
    const int coul_fond[3]   = {37, 44, 56};     // bleu-gris
    const int coul_axes[3]   = {255, 255, 255};  // white
    const int coul_trait[3]  = {51, 160,  44};   //vert_fonce
    const int coul_trait2[3] = {253, 191, 111};  //orange_clair
    const int coul_trait3[3] = {31, 120, 180};   //bleu_fonce
    const int coul_trait4[3] = {227,  26,  28};  //rouge_fonce
    
//    gdImagePtr im_fig1; /**< Image figure evolution stations en favoris*/
//    int w = 800, h = 600;  
//    im_fig1 = gdImageCreate(w, h);
//    printf("Figsize : %dpx x %dpx \n", im_fig1->sx, im_fig1->sy);
//    /* Creation du fond et de la zone de dessin a fond noir*/
//    make_background(im_fig1, pad[0], pad[1], coul_fond);
//    /* Zero des axes*/
//    const int origin_axes[2] = {pad[0], im_fig1->sy-1-pad[1]};
//    /* Quelques donnees*/
//    // Limites zone de dessin :
//    // Gauche : padX      (orig[0])
//    // Droite : Nx-1 - margX
//    // Bas    : Ny-padY-1 (orig[1])
//    // Haut   : 0 + margY
   
//    const int ptx[] = {0, 100, 200, 300, 400};
//    const int pty[] = {3, 8, 2, 1, 6};
//    const int pty2[] = {1, 4, 7, 3, 6};
//    const int pty3[] = {5, 1, 4, 9, 6};
//    /* Recupere nombre de points*/
//    size_t len_data = sizeof(ptx)/sizeof(ptx[0]);
//    /* Plot de quelques tracés*/
//    PlotLine(im_fig1, origin_axes, margin,\
//                len_data, ptx,  pty,  coul_trait, w_lines, ms);
//    PlotLine(im_fig1, origin_axes, margin,\
//                len_data, ptx, pty2, coul_trait2, w_lines, ms);
//    PlotLine(im_fig1, origin_axes, margin,\
//                len_data, ptx, pty3, coul_trait3, w_lines, ms);
//    /* Ecriture xlabel */
//    int brect[8] = {0};
//    char *xlabel = "Date";
//    int posX_xlabel = origin_axes[0]+ (w-pad[0])/2;
//    int posY_xlabel = origin_axes[1]+(pad[1]/2) + pad[1]/3;
//    printf("Pos xlabel : %d, %d \n", posX_xlabel, posY_xlabel);
//    errStringFT = gdImageStringFT(im_fig1, brect,\
//                        getCouleur(im_fig1, coul_axes), fontLabels,\
//                            labelSize, 0., posX_xlabel, posY_xlabel, xlabel);
   
//    printf("%s \n", errStringFT);
//    // /* Ecriture ylabel */
//    // char *ylabel = "Nb bornes";
//    // int posX_ylabel = pad[0]/2;
//    // int posY_ylabel = origin_axes[1]/2;    
//    // errStringFT = gdImageStringFT(im_fig1, brect, coul_axes, fontLabels,\
//    //                  26, 90., posX_ylabel, posY_ylabel, xlabel);
//    // printf("%s \b", errStringFT);
//    /* Dessiner les ticks */
//    /* Valeurs des ticks  */
//    /* Creation des axes au bord de la zone de dessin*/
//    make_support_axes(im_fig1, origin_axes, coul_axes); 