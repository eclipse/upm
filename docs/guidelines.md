# Code Commenting And Documentation Authoring Guidelines

#### [Part I. Code Commenting](#code-commenting)

[Grammar](#grammar)

- [Active vs passive voice](#voice)

- [Capitalization](#capitalization)

- [No possessive case](#possessive-case)

- [Present tense](#present-tense)

- [Second person vs the user / reader / programmer / engineer](#second-person)

- [Third person vs infinitive](#third-person)

[Punctuation](#punctuation)

- [Comma in enumerations](#comma)

- [No period at the end of description](#period)

[Specific word usage](#word-usage)

- [Abbreviations and acronyms](#abbr-acr)

- [Adjectives containing numbers](#adj-num)

- [App vs application](#app)

- [Function vs method](#func-meth)

- [Onboard vs on-board](#onboard)

- [Sensor name vs sensor model](#name-model)

- [Setup vs set up](#setup)

- [Wi-Fi vs WiFi / Wifi / Wi-fi / wifi / wi-fi](#setup)

[Trademarks](#trademarks)

- [Intel products](#intel-prod)

- [Third-party technology](#third-party)

- [Trademark + noun](#tm-noun)

#### [Part II. Documentation Authoring](#doc-authoring)

[Grammar](#da-grammar)

- [Capitalization](#da-capitalization)

[Styling](#da-styling)

- [Bolding](#da-bolding)

- [Backticks](#da-backticks)

- [Links](#da-links)

- [Numbered list vs bullet points](#da-lists)

[Specific word usage](#da-word-usage)

- [Login vs log in](#da-login)

- [Click](#da-click)

- [SSH](#da-ssh)

# <a name="code-commenting"></a>Code Commenting Guidelines

## <a name="grammar"></a>Grammar

### <a name="voice"></a>Active vs passive voice

Where possible, prefer active voice over passive.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| It can be put into the configuration mode by grounding the CONFIG pin on the transceiver. | - You can put it into the configuration mode by grounding the CONFIG pin on the transceiver.<br>- To put it into the configuration mode, ground the CONFIG pin on the transceiver.<br>- Put it into the configuration mode by grounding the CONFIG pin on the transceiver. |

### <a name="capitalization"></a>Capitalization

- Capitalize the first word in the description of an entity.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| checks to see if there is data available for reading | Checks to see if there is data available for reading |
| @param len length of the buffer | @param len Length of the buffer |

- Be consistent with the capitalization of boolean values.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| Returns True on success, false otherwise | - Returns true on success, false otherwise<br>- Returns True on success, False otherwise |

### <a name="possessive-case"></a>No possessive case

Do not use possessive case to avoid unnecessary personalization.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| method's output | - method output<br>- output of the method |

### <a name="present-tense"></a>Present tense

Use the present simple tense instead of future, past, or present perfect.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| When specified, this value will be used in computing the voltage. | When specified, this value is used in computing the voltage. |
| Once the data has been read… | Once the data is read… |

### <a name="second-person"></a>Second person vs the user / reader / programmer / engineer

Use second person when addressing the target reader of your comment.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| The user can easily override this method. | You can easily override this method. |

### <a name="third-person"></a>Third person vs infinitive

Use third-person verb forms in short descriptions of classes, methods, functions, etc., not infinitive.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| Get the proximity value from the sensor | Gets the proximity value from the sensor |

## <a name="punctuation"></a>Punctuation

### <a name="comma"></a>Comma in enumerations

Add an extra comma before the last item in a list joined by **and** / **or**.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| Returns raw values for the X, Y and Z axes. | Returns raw values for the X, Y, and Z axes. |

### <a name="period"></a>No period at the end of description

Do not put a period if the description of an entity is one sentence long.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| Returns the name of the sensor. | Returns the name of the sensor |
| Sets the frequency modulation<br>Valid values are between 10 and 160 (in kHz) | Sets the frequency modulation. Valid values are between 10 and 160 (in kHz). |
| @param millis Maximum time in milliseconds to wait for the input<br>-1 means waiting forever (default) | @param millis Maximum time in milliseconds to wait for the input. -1 means waiting forever (default). |

<font color="purple">**Exception:**</font> if the description contains more than one sentence, put periods after each sentence.

## <a name="word-usage"></a>Specific word usage

### <a name="abbr-acr"></a>Abbreviations and acronyms

- Spell out the first occurrence or the first prominent use of an abbreviation or acronym, followed by a shortened form.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| @brief API for the GP2Y0A family of IR Proximity Sensors | @brief API for the GP2Y0A family of infrared (IR) Proximity Sensors |

- Do not use Latin abbreviations.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| Works best with halved values; e.g., 1.0, 0.5, 0.25, etc. | Works best with halved values; for example, 1.0, 0.5, 0.25, and so on. |

- Know exactly what the abbreviation or acronym means to avoid unnecessary duplication.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| - LCD display<br>- ISR routine | - LCD / liquid-crystal display<br>- ISR / interrupt service routine |

### <a name="adj-num"></a>Adjectives containing numbers

Adjectives of the form **number + noun / participle** should be hyphenated. It does not matter if a number is represented by one or more digits or spelled out.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| - 4 wire stepper motor<br>- 3 axis gyroscope<br>- zero based indexing<br>- one byte register | - 4-wire stepper motor<br>- 3-axis gyroscope<br>- zero-based indexing<br>- one-byte register |

### <a name="app"></a>App vs application

Use **app** when referring to a program running on a device, and **application** when referring to a program running on a desktop / laptop computer.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| The wiki page for this device includes a link to an Android\* application that can be used to read the device via NFC. | The wiki page for this device includes a link to an Android\* app that can be used to read the device via NFC. |

### <a name="func-meth"></a>Function vs method

If a function is associated with a class, use **method** instead.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| class WheelEncoder {<br>...<br>/**<br>* Starts the counter. This function also clears<br>* the current count and resets the clock.<br>*/<br>void startCounter(); | class WheelEncoder {<br>...<br>/**<br>* Starts the counter. This method also clears<br>* the current count and resets the clock.<br>*/<br>void startCounter(); |

### <a name="onboard"></a>Onboard vs on-board

Use **onboard**.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| This ADC features an on-board reference and oscillator. | This ADC features an onboard reference and oscillator. |

### <a name="name-model"></a>Sensor name vs sensor model

Use a sensor name alone or a sensor model followed by a sensor name, not a sensor model alone.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| ADXL345 is compatible with… | - The ADXL345 3-axis digital accelerometer is compatible with…<br>- The accelerometer is compatible with… |

<font color="purple">**Exception:**</font> you can use a sensor model alone when introducing a
sensor for the first time:

ADXL345 is a 3-axis digital accelerometer…

### <a name="setup"></a>Setup vs set up

**Setup** is a noun, **set up** is a verb.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| It does not require any additional set up. | It does not require any additional setup. |
| For instructions on how to setup…, refer to... | For instructions on how to set up…, refer to... |

### <a name="wi-fi"></a>Wi-Fi vs WiFi / Wifi / Wi-fi / wifi / wi-fi

Use **Wi-Fi**.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| It was tested with the XBee\* S6B WiFi module. | It is tested with the XBee\* S6B Wi-Fi\* module. |

## <a name="trademarks"></a>Trademarks

### <a name="intel-prod"></a>Intel products

Use the correct official names of Intel products. When in doubt, check the TM names database.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| It is a 64x48 pixel OLED display that connects directly to an edison via its 80-pin connector. | It is a 64x48 pixel OLED display that connects directly to an Intel(R) Edison board via its 80-pin connector. |

**Note:** if an Intel product has a legally approved short name, you may use it in subsequent instances, after spelling it out the first time.

### <a name="third-party"></a>Third-party technology

Add an asterisk (\*) after a name to indicate a third-party trademark or registered intellectual property. If you are not sure whether an asterisk is necessary after a particular name, the rule of thumb is to put one, to be on the safe side.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| The Grove MQ2 Gas Sensor module is useful for gas leakage detection. | The Grove\* MQ2 Gas Sensor module is useful for gas leakage detection. |

### <a name="tm-noun"></a>Trademark + noun

Always follow trademarks by an appropriate noun. For a list of approved nouns for a particular trademark, check the TM names database.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| It is a 64x48 pixel OLED display that connects directly to an Intel(R) Edison via its 80-pin connector. | It is a 64x48 pixel OLED display that connects directly to an Intel(R) Edison board via its 80-pin connector. |

# <a name="doc-authoring"></a>Documentation Authoring Guidelines

## <a name="da-grammar"></a>Grammar

### <a name="da-capitalization"></a>Capitalization

For titles, use sentence capitalization.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| Add a New Device | Add a new device |

## <a name="da-styling"></a>Styling

### <a name="da-bolding"></a>Bolding

For GUI elements and file names, use bolding instead of quotes.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| In the “Environment Variables” window, click “OK”. | In the **Environment Variables** window, click **OK**. |
| Copy the “example.zip” archive into the installation directory. | Copy the **example.zip** archive into the installation directory. |

### <a name="da-backticks"></a>Backticks

Enclose program commands, code blocks, and file paths in backticks (\`).

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| To create a new device, use the **create-thing** command. | To create a new device, use the \`create-thing\` (rendered as `create-thing`) command. |
| Go to **C:\Users\me\Documents\GitHub\intel-iot-examples-mqtt\support\aws**. | Go to \`C:\Users\me\Documents\GitHub\intel-iot-examples-mqtt\support\aws\` (rendered as `C:\Users\me\Documents\GitHub\intel-iot-examples-mqtt\support\aws`). |

### <a name="da-links"></a>Links

Do not use embedded links to third-party websites.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| Create an account on [Microsoft Azure](https://azure.microsoft.com/en-us), if you do not yet have one. | Create an account on [https://azure.microsoft.com/en-us](https://azure.microsoft.com/en-us), if you do not yet have one. |

### <a name="da-lists"></a>Numbered list vs bullet points

- For a logical sequence of steps, use a numbered list.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| When running your C++ code on the Edison, you need to set the MQTT parameters in Eclipse. Go to "Run configurations", and change the "Commands to execute before application" to the following:<br>…<br>Click on the "Apply" button to save these settings.<br>Click on the "Run" button to run the code on the Edison. | When running your C++ code on the Intel® Edison board, you need to set the MQTT\* client parameters in Eclipse\*. To do that:<br>1. Go to **Run configurations** and, in the **Commands to execute before application** field, type the following:<br>…<br>2. Click the **Apply** button to save these settings.<br>3. Click the **Run** button to run the code on your board. |

- For a list of equally important options, use a bulleted list.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| From this exercise, developers will learn how to:<br>1. Connect the Intel® Edison board...<br>2. Run these code samples in the Intel® System Studio IoT Edition...<br>3. Set up a web application server... | From this exercise, developers will learn how to:<br>- Connect the Intel® Edison board…<br>- Run these code samples in the Intel® System Studio IoT Edition…<br>- Set up a web application server… |

## <a name="da-word-usage"></a>Specific word usage

### <a name="da-login"></a>Login vs log in

**Login** is a noun, **log in** is a verb.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| Provide your log in and password. | Provide your login and password. |
| Login to your account. | Log in( )to your account. |

### <a name="da-click"></a>Click

Omit **on** after **click**.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| Click on **Advanced system settings**. | Click **Advanced system settings**. |

### <a name="da-ssh"></a>SSH

Do not use SSH as a verb. It is considered slang and is to be avoided.

| <font color="red">**Incorrect**</font> | <font color="green">**Correct**</font> |
| --- | --- |
| SSH into your Intel® Edison board. | Establish an SSH connection to your Intel® Edison board. |
