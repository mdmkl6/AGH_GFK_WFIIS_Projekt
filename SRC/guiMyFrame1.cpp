/////////////////////////////
// Projekt GFK04
// Michal Domin
// Damian Raczynski
// Kacper Osuch
/////////////////////////////
#include "guiMyFrame1.h"

guiMyFrame1::guiMyFrame1(wxWindow* parent)
    :
    MyFrame1(parent)
{
    minVal = 0;
    maxVal = 0;
    minDim = 0;
    maxDim = 0;
    loadGauge->Hide();
    MemoryBitmap.Create(600, 600, 24);
    image.Create(600, 600);
    modifier = 1;
    nextSliceButton->Disable();
    previousSliceButton->Disable();
    vectorChoice->Disable();
    saveButton->Disable();
}

void guiMyFrame1::loadButtonClick(wxCommandEvent& event)
{
    wxFileDialog WxOpenFileDialog(this, wxT("Wybierz plik"), wxT(""), wxT(""), wxT("Plik data (*.dat)|*.dat"), wxFD_OPEN | wxFD_FILE_MUST_EXIST);

    if (WxOpenFileDialog.ShowModal() == wxID_OK) {
        double x, y, z, val;

        std::ifstream in(WxOpenFileDialog.GetPath().ToAscii());
        if (in.is_open()) {
            data.clear();
            datas.clear();
            ix = 0;
            iy = 1;
            iz = 1;
            if (!in.eof()) {
                in >> x >> y >> z >> val;
                data.push_back({ x, y, z, val });
                minVal = val;
                maxVal = val;
                minDim = std::min({ x, y, z });
                maxDim = std::max({ x, y, z });
            }
            while (!in.eof()) {
                in >> x >> y >> z >> val;
                data.push_back({ x, y, z, val });
                if (val < minVal) minVal = val;
                if (val > maxVal) maxVal = val;
                minDim = std::min({ x, y, z, minDim });
                maxDim = std::max({ x, y, z, maxDim });
            }
            in.close();
        }
        std::sort(data.begin(), data.end(), [this](std::array<double, 4> a, std::array<double, 4> b) {return compare(a, b); });
        int xi = -1, yi = 1;
        for (auto& el : data)
        {
            if (xi == -1)
            {
                datas.push_back({ {{el[0],0}}, {{el[1],0},{el[2],el[3]}} });
                xi++;
            }
            else if (datas[xi][0][0][0] == el[0])
            {
                if (datas[xi][yi][0][0] == el[1])
                {
                    datas[xi][yi].push_back({ el[2], el[3] });
                }
                else
                {
                    datas[xi].push_back({ {el[1],0},{el[2],el[3]} });
                    yi++;
                }
            }
            else
            {
                datas.push_back({ {{el[0],0}}, {{el[1],0},{el[2],el[3]}} });
                xi++;
                yi = 1;
            }
        }
#ifdef DEBUG_PROCESS
        std::cout << "minDim " << minDim << " maxDim " << maxDim << std::endl;
#endif
        currentSlice = 1;
        modifier = minDim + (2*maxDim - 2*minDim) / 6;
        DrawMap();
        previousSliceButton->Disable();
        nextSliceButton->Enable();
        vectorChoice->Enable();
        saveButton->Enable();
        
    }
}

void guiMyFrame1::saveButtonClick(wxCommandEvent& event)
{
    wxFileDialog saveFileDialog(this, _("Zapisz jako:"), _(""), _(""), _("Plik BMP(*.bmp)|*.bmp"), wxFD_SAVE | wxFD_OVERWRITE_PROMPT);
    if (saveFileDialog.ShowModal() == wxID_CANCEL)
        return;
    int w, h;
    wxClientDC MyDC(drawPanel);
    wxMemoryDC MyBitmapDC;
    drawPanel->GetClientSize(&w, &h);
    wxBitmap MyBitmap(w, h, 24);
    MyBitmapDC.SelectObject(MyBitmap);
    MyBitmapDC.Blit(0, 0, w, h, &MyDC, 0, 0);
    MyBitmap.ConvertToImage().SaveFile(saveFileDialog.GetPath(), wxBITMAP_TYPE_BMP);
}

void guiMyFrame1::OnChoice(wxCommandEvent& event)
{
    currentSlice = 1;
    modifier = minDim + (2*maxDim - 2*minDim) / 6;
    nextSliceButton->Enable();
    previousSliceButton->Disable();
    DrawMap();
#ifdef DEBUG_PROCESS
    std::cout << "choice " << vectorChoice->GetSelection() << std::endl;
#endif
}

void guiMyFrame1::previousSliceClick(wxCommandEvent& event)
{
    currentSlice--;
    if (currentSlice <= 1) previousSliceButton->Disable();
    nextSliceButton->Enable();
    modifier = minDim + currentSlice * (2 * maxDim - 2 * minDim) / 6;
    DrawMap();
}

void guiMyFrame1::nextSliceClick(wxCommandEvent& event)
{
    currentSlice++;
    if (currentSlice >= 5) nextSliceButton->Disable();
    previousSliceButton->Enable();
    modifier = minDim + currentSlice * (2 * maxDim - 2 * minDim) / 6;
    DrawMap();
}

std::vector<std::array<double, 3>> guiMyFrame1::plaszczyzna() const
{
    std::array<double, 3> x0 = {};
    double mnoznik = modifier, delta, max, delta2;
    std::vector<std::array<double, 3>> w;
    switch (vectorChoice->GetSelection())
    {
    case 0: // [1,0,0]
        if (mnoznik < minDim || mnoznik >2 * maxDim - minDim)
            mnoznik = maxDim;
        delta = (datas[0][datas[0].size() - 1][0][0] - datas[0][1][0][0]) / 600;
        for (double x = minDim; x <= maxDim; x += delta)
        {
            for (double y = minDim; y <= maxDim; y += delta)
                w.push_back({ (mnoznik + minDim) / 2,x, y });
        }
        break;

    case 1: // [0,1,0]
        if (mnoznik < minDim || mnoznik >2 * maxDim - minDim)
            mnoznik = maxDim;
        delta = (datas[0][datas[0].size() - 1][0][0] - datas[0][1][0][0]) / 600;
        for (double x = minDim; x <= maxDim; x += delta)
        {
            for (double y = minDim; y <= maxDim; y += delta)
                w.push_back({ x,(mnoznik + minDim) / 2, y });
        }
        break;

    case 2: // [0,0,1]
        if (mnoznik < minDim || mnoznik >2 * maxDim - minDim)
            mnoznik = maxDim;
        delta = (datas[0][datas[0].size() - 1][0][0] - datas[0][1][0][0]) / 600;
        for (double x = minDim; x <= maxDim; x += delta)
        {
            for (double y = minDim; y <= maxDim; y += delta)
                w.push_back({ x,y,(mnoznik + minDim) / 2 });
        }
        break;

    case 3: // [1,1,0]
        if (mnoznik <= maxDim && mnoznik > minDim)
            x0 = { minDim,minDim,mnoznik };
        else if (mnoznik < 2 * maxDim - minDim)
            x0 = { mnoznik - maxDim + minDim,minDim,maxDim };
        else
            x0 = { minDim,minDim,maxDim };
        max = (x0[2] - x0[0]);
        delta = max / 600;
        delta2 = (datas[0][datas[0].size() - 1][0][0] - datas[0][1][0][0]) / 600;
        for (double x = 0; x <= max; x += delta)
        {
            for (double y = minDim; y <= maxDim; y += delta2)
                w.push_back({ x0[0] + x ,x0[2] - x, y });
        }
        break;

    case 4: // [1,0,1]
        if (mnoznik <= maxDim && mnoznik > minDim)
            x0 = { minDim,minDim,mnoznik };
        else if (mnoznik < 2 * maxDim - minDim)
            x0 = { mnoznik - maxDim + minDim,minDim,maxDim };
        else
            x0 = { minDim,minDim,maxDim };
        max = (x0[2] - x0[0]);
        delta = max / 600;
        delta2 = (datas[0][datas[0].size() - 1][0][0] - datas[0][1][0][0]) / 600;
        for (double x = 0; x <= max; x += delta)
        {
            for (double y = minDim; y <= maxDim; y += delta2)
                w.push_back({ x0[0] + x ,y, x0[2] - x });
        }
        break;

    case 5: // [0,1,1]
        if (mnoznik <= maxDim && mnoznik > minDim)
            x0 = { minDim,minDim,mnoznik };
        else if (mnoznik < 2 * maxDim - minDim)
            x0 = { mnoznik - maxDim + minDim,minDim,maxDim };
        else
            x0 = { minDim,minDim,maxDim };
        max = (x0[2] - x0[0]);
        delta = max / 600;
        delta2 = (datas[0][datas[0].size() - 1][0][0] - datas[0][1][0][0]) / 600;
        for (double x = 0; x <= max; x += delta)
        {
            for (double y = minDim; y <= maxDim; y += delta2)
                w.push_back({ y,x0[0] + x, x0[2] - x });
        }
        break;
    }
    return w;
}

double guiMyFrame1::odl(std::array<double, 4>& a, std::array<double, 4>& b)
{
    if (abs(a[0] - b[0]) < 0.001 && abs(a[1] - b[1]) < 0.001 && abs(a[2] - b[2]) < 0.001)
        return -1;
    return sqrt(pow(a[0] - b[0], 2) + pow(a[1] - b[1], 2) + pow(a[2] - b[2], 2));
}

bool guiMyFrame1::compare(std::array<double, 4> a, std::array<double, 4> b)
{
    if (fabs(a[0] - b[0]) < 0.01)
    {
        if (fabs(a[1] - b[1]) < 0.01)
        {
            if (a[2] < b[2])
                return true;
            else return false;
        }
        else if (a[1] < b[1])
            return true;
        else return false;
    }
    else if (a[0] < b[0])
        return true;
    else return false;

}

double guiMyFrame1::getVal(std::array<double, 3>& x0)
{
    if (x0[0] >= datas[ix][0][0][0] && x0[0] <= datas[ix + 1][0][0][0])
    {
        if (x0[1] >= datas[ix][iy][0][0] && x0[1] <= datas[ix][iy + 1][0][0])
        {
            if (x0[2] >= datas[ix][iy][iz][0] && x0[2] <= datas[ix][iy][iz + 1][0])
            {
                double s = 0;
                double so = 0;
                double o;
                std::array<double, 4> v = { x0[0],x0[1],x0[2],0 };
                std::array<double, 4> v2 = {};
                for (int i = 0; i <= 1; i++)
                {
                    for (int j = 0; j <= 1; j++)
                    {
                        for (int k = 0; k <= 1; k++)
                        {
                            v2 = { datas[ix + i][0][0][0],datas[ix + i][iy + j][0][0],datas[ix + i][iy + j][iz + k][0],0 };
                            o = odl(v, v2);
                            if (o == -1) return datas[ix + i][iy + j][iz + k][1];
                            s += datas[ix + i][iy + j][iz + k][1] / o;
                            so += 1 / o;
                        }
                    }
                }
                return s / so;
            }
            else if (iz + 2 == datas[0][1].size())
            {
                iz = 1;
                return getVal(x0);
            }
            else
            {
                iz++;
                return getVal(x0);
            }
        }
        else if (iy + 2 == datas[0].size())
        {
            iy = 1;
            return getVal(x0);
        }
        else
        {
            iy++;
            return getVal(x0);
        }
    }
    else if (ix + 2 == datas.size())
    {
        ix = 0;
        return getVal(x0);
    }
    else
    {
        ix++;
        return getVal(x0);
    }
}

void guiMyFrame1::DrawMap()
{
    wxMemoryDC memDC;
    memDC.SelectObject(MemoryBitmap);
    memDC.SetBackground(*wxWHITE_BRUSH);
    memDC.Clear();

    std::vector<std::array<double, 3>> plasc = plaszczyzna();

#ifdef DEBUG_PROCESS
    std::cout << "Jest plaszczyzna\n";
#endif

    loadGauge->Show();
    double color;
    unsigned char* points = image.GetData();
    for (unsigned x = 0; x < 600; x++) {
        for (unsigned y = 0; y < 600; y++) {
            color = ((getVal(plasc[600 * x + y]) - minVal) / (maxVal - minVal));
            points[y * 600 * 3 + x * 3 + 0] = static_cast<unsigned char> (255 * color);
            points[y * 600 * 3 + x * 3 + 1] = 0;
            points[y * 600 * 3 + x * 3 + 2] = static_cast<unsigned char> (255 - 255 * color);
        }
        loadGauge->SetValue(x + 1);
    }
    loadGauge->Hide();
    const wxBitmap tmp_bmp(image, 24);
    memDC.DrawBitmap(tmp_bmp, 0, 0);

#ifdef DEBUG_PROCESS
    std::cout << "Drawn" << std::endl;
#endif

    Repaint();
}

void guiMyFrame1::Repaint()
{
    wxMemoryDC memDC;
    memDC.SelectObject(MemoryBitmap);
    wxClientDC dc(drawPanel);
    dc.Blit(0, 0, 600, 600, &memDC, 0, 0);
    memDC.SelectObject(wxNullBitmap);

#ifdef DEBUG_PROCESS
    std::cout << "Repainted" << std::endl;
#endif
}