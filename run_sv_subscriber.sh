
 #lancement de n flux de SV_publisher simultanément 
  
 #                            exécutable     sv_sub  intf réseau    appid_sv  file_saving nbre_subscriber
  sudo gnome-terminal   -x  ./sv_subscriber   eno1   0x4000       "./test_bool.csv"    &
  #sudo gnome-terminal   -x  ./sv_subscriber eno1    0x4001       "./test_bool2.csv"   &
  #sudo gnome-terminal   -x  ./sv_subscriber  eno1   0x4002      "./test_natif/subscribe5/time_stamp3.csv" &
  #sudo gnome-terminal   -x  ./sv_subscriber  eno1   0x4003      "./test_natif/subscribe5/time_stamp4.csv" &
  #sudo gnome-terminal   -x  ./sv_subscriber  eno1   0x4004      "./test_natif/subscribe5/time_stamp5.csv"&
