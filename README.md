# 🔒 Coffre Fort ESP32 page web

Projet complet : Code Arduino + Démo.

### 1. code
Dans `/Code` : `sketch_cofre_fort_v3` à ouvrir avec Arduino IDE.
### 2. Démo
 OUVRIR demo_photo_video pour voir image du montage et video demo.

### 3. Composants
 
ESP32-WROOM-32, LCD I2C, Servo SG90, Buzzer, 2 LED(rouge,vert) , 2 Résistance,une plaquette d'éssaie et des fils de connexion


### FONTIONNEMENT
 ## 1. 
    A l'allumage du dispositif vous allumez le wifi de votre telephone ordinateur ou tablette
    vous verez le nom Coffre_V3 vous vous connecter la dessu et le mot de passe es 12345678
    ensuite vous iriez dans un navigateur et vous entrer l'IP 192.168.4.1
    Et vous verez la page
  
 ## 2.
     Sur la page web vous verez le champs entrer le code pin si vous entrer le bon  code il  y as un message d'acces 
     au contraire message erreur
     
 ## 3.
     Et la dexieme partie c'est le changement du code et pour sa il te demande l'ancien code si tu entre le correct
     tu peut maintenant entrer ton nouveau code maintenant
 
## 4. 
    Apres trois nouveau tentative le systeme se bloque pendant 1 minute
 
