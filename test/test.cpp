#include <list>
#include <gtest/gtest.h>
#include <wx/wx.h>
#include "MainFrame.h"

int ARGC;
char** ARGV;

int main(int argc, char *argv[])
{
#ifdef _WIN32
	SetProcessDPIAware();
#endif // _WIN32
	ARGC = argc;
	ARGV = argv;
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

TEST(Test1, test1) {
    EXPECT_EQ(1, 1);
}

struct Tasks
{
	std::list<std::function<void()> > q;
	Tasks() {}
	Tasks(std::function<void()> f)
	{
		q.push_back(f);
	}
	Tasks& then(const std::function<void()>& f)
	{
		q.push_back(f);
		return *this;
	}
};

class TestApp : public wxApp
{
public:

	TestApp()
	{
		ms = 500;
	}

	bool OnInit() override
	{
		m_timer.SetOwner(this);
		return true;
	}

	int OnRun() override
	{
		m_timer.StartOnce(ms);
		return wxApp::OnRun();
	}

	Tasks tasks;

private:

	void OnTimer(wxTimerEvent&)
	{
		std::function<void()> f;
		if (!tasks.q.empty())
		{
			f = tasks.q.front();
			tasks.q.pop_front();
		}
		if (f)
		{
			f();
		}
		if (!tasks.q.empty())
		{
			m_timer.StartOnce(ms);
			return;
		}
		//End of tasks
		wxWindow* top = wxTheApp->GetTopWindow();
		if (top)
		{
			top->Close();
		}
	}

	int ms;
	wxTimer m_timer;
	wxDECLARE_EVENT_TABLE();
};
		
wxBEGIN_EVENT_TABLE(TestApp, wxApp)
	EVT_TIMER(wxID_ANY, TestApp::OnTimer)
wxEND_EVENT_TABLE()

struct TestAppFixture : public testing::Test
{
	virtual void SetUp() override
	{
		app = new TestApp();
		wxApp::SetInstance(app);
		wxEntryStart(ARGC, ARGV);
		app->OnInit();
	}

	virtual void TearDown() override
	{
		app->OnExit();
		wxEntryCleanup();
	}

	void Run(Tasks tasks)
	{
		app->tasks = tasks;
		app->OnRun();
	}

	TestApp* app;
};

struct TestMainFrameFixture : public TestAppFixture
{
	virtual void SetUp() override
	{
		TestAppFixture::SetUp();
		main_frame = new MainFrame();
		main_frame->Show();
	}

	MainFrame* main_frame;
};

TEST_F(TestMainFrameFixture, test_input_device_panel) {

	Run(Tasks([&]() {
		EXPECT_EQ(main_frame->input_device_panel->device_list_ctrl->GetCount(), 0);
	}));
}
