#include "FindWaterLevel.h"
#include "WaterLevel.h"
#include "gbufferedimage.h"
using namespace std;

namespace {
    const string kOutputFile = "result.png";
    const string kBackgroundColor = "#802040";

    const string kWaterHeightText = "Water Height: ";
    const string kFloodButtonText = "Go!";

    const string kRenderingText   = "Rendering the result...";
    const string kBadNumberText   = "Please enter a real number.";

    const string kLoadingText     = "Loading terrain...";

    const size_t kTextInputSize = 15;

    /* Color to use for water. */
    const int kUnderwaterColor = GBufferedImage::createRgbPixel(0, 49, 83); // Prussian blue

    /* The rest of the map is colored according to the following scheme. The heights are mapped
     * to real numbers between 0 and 1. Those real numbers are then used to interpolate between
     * a fixed series of color points marked off at various intervals.
     *
     * The RGBPoint type represents a particular RGB color annotated with a threshold value
     * between 0 and 1 indicating where that color sits.
     */
    struct RGBPoint {
        int red, green, blue;
        double threshold;
    };

    /* The actual colors to use to draw the map, annotated with their threshold values. */
    const vector<RGBPoint> kColors = {
        {   0, 102,   0, 0.0  },   // Pakistan green
        { 154, 205,  50, 0.1  },   // Chartreuse
        { 251, 236,  93, 0.25 },   // Maize
        { 212, 175,  55, 0.4  },   // Metallic gold
        { 166,  60,  20, 1.01 }    // Sienna. The 1.01 here is to ensure we cover rounding errors.
    };

    /* Linearly interpolates between two quantities. The progress variable runs between 0 and 1. */
    int interpolate(int from, int to, double progress) {
        return from + (to - from) * progress;
    }

    /* Given an absolute height and the range of heights associated with a given range, returns
     * the color associated with that height.
     */
    int colorFor(double height, bool underwater, double lowest, double highest) {
        /* If the point is underwater, it's always given the water color. */
        if (underwater) return kUnderwaterColor;

        /* Map height to [0, 1]. */
        double alpha = (height - lowest) / (highest - lowest);

        /* Figure out which points we're between. */
        for (int i = 1; i < kColors.size(); i++) {
            if (alpha <= kColors[i].threshold) {
                /* Progress is measured by how far between the two points we are. 0.0 means
                 * "completely at the left end. 1.0 means "completely at the right end."
                 */
                double progress = (alpha - kColors[i - 1].threshold) /
                                  (kColors[i].threshold - kColors[i - 1].threshold);

                /* Interpolate between those color points to get our overall color. */
                int red   = interpolate(kColors[i - 1].red,   kColors[i].red,   progress);
                int green = interpolate(kColors[i - 1].green, kColors[i].green, progress);
                int blue  = interpolate(kColors[i - 1].blue,  kColors[i].blue,  progress);

                return GBufferedImage::createRgbPixel(red, green, blue);
            }
        }

        /* This code is unreachable. */
        error("Impossible alpha: " + to_string(alpha));
        return 0;
    }

    /* Draws the given image to the output file. This is done because GBufferedImage is an efficient
     * way to push the image to the backend, but doesn't support resizing (TODO: validate this).
     * We therefore dump it to a file and reload it later as a GImage, which does support resizing.
     */
    void renderToFile(const Grid<double>& heights, const Grid<bool>& underwater) {
        double lowest  = *min_element(heights.begin(), heights.end());
        double highest = *max_element(heights.begin(), heights.end());

        Grid<int> pixels(heights.numRows(), heights.numCols());
        for (int row = 0; row < heights.numRows(); row++) {
            for (int col = 0; col < heights.numCols(); col++) {
                pixels[row][col] = colorFor(heights[row][col], underwater[row][col], lowest, highest);
            }
        }

        GBufferedImage image;
        image.fromGrid(pixels);
        image.save(kOutputFile);
    }

    /* Generates a semi-humorous message to pass the time as everything computes. */
    string floodMessage() {
        switch (rand() % 4) {
            case 0:  return "Forecasting the flood...";
            case 1:  return "Watering the world...";
            case 2:  return "Deploying the deluge...";
            default: return "Siumulating the surge...";
        }
    }
}

/* We use the .terrain file type. */
string FindWaterLevel::fileExtension() {
    return ".terrain";
}

FindWaterLevel::FindWaterLevel(istream& input, GWindow& window)
  : heightDesc (new GLabel(kWaterHeightText),   window, "NORTH"),
    heightField(new GTextField(kTextInputSize), window, "NORTH"),
    solveButton(new GButton(kFloodButtonText),  window, "NORTH"),
    statusLine (new GLabel(""),                 window, "SOUTH")
{
    heightField->setText("0.0");

    statusLine->setText(kLoadingText);
    plain = loadFloodplain(input);

    runFlood(0.0);
}

/* Runs a flood starting from the given height. */
void FindWaterLevel::runFlood(double height) {
    statusLine->setText(floodMessage());
    underwater = simulateFlood(plain, height);
    requestRepaint();
}

/* Renders the result of the flood. */
void FindWaterLevel::repaint(GWindow& window) {
    statusLine->setText(kRenderingText);

    /* Render the image to disk, then load the image. */
    renderToFile(plain.heights, underwater);
    GImage image(kOutputFile);

    /* Rescale the image to fit into the window. */
    double aspectRatio = image.getWidth() / image.getHeight();
    if (aspectRatio > window.getCanvasWidth() / window.getCanvasHeight()) {
        image.scale(window.getCanvasWidth() / image.getWidth());
    } else {
        image.scale(window.getCanvasHeight() / image.getHeight());
    }

    /* Clear the display. */
    window.setColor(kBackgroundColor);
    window.fillRect(0, 0, window.getCanvasWidth(), window.getCanvasHeight());

    /* Draw the image. */
    window.draw(&image, (window.getCanvasWidth() - image.getWidth()) / 2, (window.getCanvasHeight() - image.getHeight()) / 2);
    window.repaint();

    statusLine->setText("");
}

void FindWaterLevel::actionPerformed(GObject* source) {
    if (source == heightField.get() || source == solveButton.get()) {
        /* Validate that what was entered was indeed a real number. */
        double height;
        try {
            height = stringToReal(heightField->getText());
        } catch (const exception& e) {
            statusLine->setText(kBadNumberText);
        }
        runFlood(height);
    }
}
