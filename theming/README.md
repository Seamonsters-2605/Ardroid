The "Holo" or "Material" theme can optionally be applied to the app to significantly improve the appearance. This is done by editing the APK file with AppToMarket. If you choose not to apply this theme, you should disable "TitleVisible" on every screen of the app - without the theme the app looks better without a title on each screen. However, the custom theme requires TitleVisible to be enabled (it automatically hides it).

# Applying the Holo theme

These instructions are based on [this](http://baasapps.com/app-inventor/theme-material-app-inventor-2/#.WePTKDBlBPa) page. But there are some changes.

- Download [AppToMarket](https://sourceforge.net/projects/ailivecomplete/files/AppToMarket_v41.zip/download)
- Open `AppToMarket_v4.1.jar`
- On the Certificate Details tab, enter the following information
    - Your Name
    - Organization (Seamonsters Robotics)
    - Clear the Department, City, State fields
    - Country: USA
    - Keep the same Key Size
    - Keystore file: point to the location of `android.keystore` (app inventor will generate this file for you if you click Projects > Export keystore)
    - Alias `androidkey` for a AI keystore
    - Key Pwd: `android` for a AI keystore
- Go to the next tab. Browse for the APK and decompile.
- Enter the following information:
    - Version: the APK version in app inventor
    - Version Name: the version name in app inventor
    - Min SDK Ver: 8
    - Target SDK Ver: 0
    - Leave the rest as default
- Click Update Manifest
- Click Edit Manifest
    - Find the <application> tag, near the top. Change the value of `android:theme` to `@style/Theme.Mytheme` (notice there's no `android:` prefix)
    - Find the last <activity> tag near the bottom, for the "ListPickerActivity." Change `android:theme` to `@style/Theme.Mytheme.Dialog`.
    - Save and Close
- Replace `1.apk` in `C:\Users\...\apktool\Framework\` with the custom `1.apk` file from this repo
- Go to `..\AppToMarket_v40\temp_myDecompiles\res\`. Delete the existing `values` folder. Copy over the custom `values`, `values-v11`, and `values-v21`. `values-v11` has the Holo theme and `values-v21` has the Material theme, so if you don't want one of those, don't copy it. Copy files from the repo's `drawable` folder to the app's `drawable` folder (but leave the icon there).
- Click Create New .apk
    - If there's an error, you didn't enter the theme name right or didn't copy the values/drawable files correctly. Or maybe you just need to Decompile again and start over.
- Click Sign
    - If there's an error, double check the keystore info above
- Click Verify
- Click Zip Align
- The new `Ardroid_new_zipAlign.apk` file is the one you install/publish

## Troubleshooting

- If the app crashes as soon as you open it
    - Make sure every screen has a title and TitleVisible is enabled
- If only some screen of the app crash
    - Again, make sure every screen has a title and TitleVisible is enabled
    - Sliders cause problems sometimes, but there are things in the values/drawable folders that are supposed to fix this. Make sure everything was copied correctly.

If you continue to have problems, you could give up on having a nice theme and delete `values-v11` and `values-v21`.
